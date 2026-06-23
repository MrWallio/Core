#include "pch.h"
#include "../Public/Version.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/Engine.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/GameInstance.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/LocalPlayer.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

std::string Version::VersionString = "";
double Version::Engine_Version = 0;
double Version::Fortnite_Version = 0.0;
int Version::Fortnite_CL = 0;

void Version::SetupVersion() {
    VersionString = UKismetSystemLibrary::GetEngineVersion().ToString();
    if (!VersionString.empty()) {
        if (!VersionString.contains("Live") && !VersionString.contains(("Next")) && !VersionString.contains(("Cert")) && VersionString.contains(("Release"))) {
            size_t releaseDash = VersionString.find("Release-");
            size_t clDash = VersionString.find("-CL-");

            if (releaseDash == std::string::npos || clDash == std::string::npos)
            {
                size_t engineDash = VersionString.find('-');
                size_t clPlus = VersionString.find("+++", engineDash);
                releaseDash = VersionString.find("Release-", clPlus);

                if (engineDash != std::string::npos && clPlus != std::string::npos && releaseDash != std::string::npos)
                {
                    std::string engineStr = VersionString.substr(0, engineDash);
                    std::string clStr = VersionString.substr(engineDash + 1, clPlus - (engineDash + 1));
                    std::string verStr = VersionString.substr(releaseDash + 8);

                    if (engineStr.find('.') != engineStr.rfind('.'))
                        engineStr = engineStr.substr(0, engineStr.rfind('.'));

                    Fortnite_Version = std::stod(verStr);
                    Fortnite_CL = std::stoi(clStr);
                    Engine_Version = std::stod(engineStr);

                    return;
                }

                Log("Failed to parse version!");
                return;
            }

            std::string verStr = VersionString.substr(releaseDash + 8, clDash - (releaseDash + 8));
            std::string clStr = VersionString.substr(clDash + 4);

            Fortnite_Version = std::stod(verStr);
            Fortnite_CL = std::stoi(clStr);
        }
        else if (VersionString.contains("Cert") || VersionString.contains("Live")) {
            size_t firstDash = VersionString.find('-');
            size_t firstPlus = VersionString.find('+');

            if (firstDash != std::string::npos && firstPlus != std::string::npos) {
                std::string engineStr = VersionString.substr(0, firstDash);

                if (engineStr.find('.') != engineStr.rfind('.')) {
                    engineStr = engineStr.substr(0, engineStr.rfind('.'));
                }

                std::string clStr = VersionString.substr(firstDash + 1, firstPlus - (firstDash + 1));

                Engine_Version = std::stod(engineStr);
                Fortnite_CL = std::stoi(clStr);

                if (Fortnite_CL == 3700114) {
                    Fortnite_Version = 1.72;
                }
                else if (Fortnite_CL == 3724489) {
                    Fortnite_Version = 1.8;
				}
                else if (Fortnite_CL == 3729133) {
					Fortnite_Version = 1.81;
                }
                else if (Fortnite_CL == 3741772) {
                    Fortnite_Version = 1.82;
                }
                else if (Fortnite_CL == 3757339) {
                    Fortnite_Version = 1.9;
				}
				else if (Fortnite_CL == 3775276) {
					Fortnite_Version = 1.91;
                }
                else if (Fortnite_CL == 3790078) {
					Fortnite_Version = 1.10;
                }
                else if (Fortnite_CL == 3807424) {
                    Fortnite_Version = 1.11;
                }
                else if (Fortnite_CL == 3640489) {
                    Fortnite_Version = 1.63;
                }
                else if (Fortnite_CL == 3825894) {
                    Fortnite_Version = 2.1;
                }
                else {
                    Log("Unknown version, cannot determine Fortnite version!");
                }
            }
        }
    }
    else {
        Log("VersionString is nullptr");
    }
}