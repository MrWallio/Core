#include "pch.h"
#include "../Public/Client.h"

#include "../Public/Configuration.h"
#include "../Public/Finder.h"
#include "../Public/Utils.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectArray.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/Engine.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Core/Public/CoreGlobals.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/GameInstance.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/GameViewportClient.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/LocalPlayer.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"

#include "FortniteGame/Public/FortManager/FortCheatManager.h"

void Client::Init(FCoreConfig& Config) {
    InitConsole(Config);

    Finder::FindGUObjectArray();
    Finder::FindGIsClient();
    Finder::FindGIsServer();
    Finder::FindGEngine();
    Finder::FindGWorld();

    Sleep(1000);

    GEngine = reinterpret_cast<UEngine**>(ImageBase + ServerOffsets::GEngine);
    GWorld = reinterpret_cast<UWorld**>(ImageBase + ServerOffsets::GWorld);
    CoreGlobals::Init();

    Version::SetupVersion();
    Log(std::format("ImageBase: 0x{:X}", ImageBase).c_str());
    Log("FullVersion: " + Version::VersionString);
    Log(std::format("Engine Version: {}", Version::Engine_Version));

    if (Version::Fortnite_Version) {
        Log(std::format("Fortnite Build: {:.2f}", Version::Fortnite_Version));
        SetConsoleTitleA(std::format("CoreClient ({:.2f}) | Starting...", Version::Fortnite_Version).c_str());
    }
    Log(std::format("Fortnite CL: {}", Version::Fortnite_CL));

    Finder::SetupOffsets();

    UEngine* Engine = UEngine::GetEngine();
    if (!Engine) {
        Log("Failed to get GEngine, is the game fully loaded?");
        return;
    }

    if (Config.bSetClientLogVerbosity) {
        Utils::SetLogVerbosity();
    }

    if (!Config.bIsProd) {
        UObject* Console = UGameplayStatics::SpawnObject(Engine->ConsoleClass.Get(), Engine->GameViewport);
        if (Console) {
            Engine->GameViewport->ViewportConsole = (UConsole*)Console;
            Log("Successfully spawned console: " + Console->GetName().ToString());
        }
        else {
            Log("Failed to spawn console!");
        }

        CreateThread(0, 0, ClientThread, 0, 0, 0);
    }

    SetConsoleTitleA(std::format("CoreClient ({:.2f})", Version::Fortnite_Version).c_str());
}

void Client::InitConsole(FCoreConfig& Config)
{
    if (Config.bEnableClientConsole) {
        AllocConsole();
        FILE* fptr;
        freopen_s(&fptr, "CONOUT$", "w+", stdout);
    }
    SetConsoleTitleA("CoreClient (Finding Build) | Starting...");
    Log("Welcome to CoreClient, Made with love by Pongo_x86!");
}

DWORD Client::ClientThread(LPVOID) {
    while (true)
    {
        UWorld* World = UWorld::GetWorld();

        if (World && World->OwningGameInstance)
        {
            if (World->OwningGameInstance->LocalPlayers.Num() > 0)
            {
                APlayerController* PlayerController = World->OwningGameInstance->LocalPlayers[0]->PlayerController;
                if (PlayerController && !PlayerController->CheatManager)
                {
                    const UClass* CheatClass = PlayerController->CheatClass.Get() ? PlayerController->CheatClass.Get() : UFortCheatManager::StaticClass();
                    Log("Spawning CheatManager: " + CheatClass->GetName().ToString() + " for PlayerController: " + PlayerController->GetName().ToString());
                    UObject* CheatManager = UGameplayStatics::SpawnObject(CheatClass, PlayerController);
                    if (CheatManager) {
                        PlayerController->CheatManager = (UCheatManager*)CheatManager;
                        Log("Successfully spawned CheatManager: " + CheatManager->GetName().ToString());
                    }
                    else
                    {
                        Log("Failed to spawn CheatManager!");
                    }
                }
            }
        }
    }
}