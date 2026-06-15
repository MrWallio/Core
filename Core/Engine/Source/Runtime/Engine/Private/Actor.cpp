#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Core/Public/Math/TransformNonVectorized.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Pawn.h"
#include "Engine/Plugins/Online/OnlineSubsystemUtils/Source/OnlineSubsystemUtils/Public/OnlineBeaconClient.h"

ENetMode AActor::InternalGetNetMode(AActor* This)
{
	FCoreConfig& Config = ConfigurationManager::GetConfig();
	if (Config.bListenServer) {
		return NM_ListenServer;
	}

	return NM_DedicatedServer;
}

void AActor::FinishSpawning(const FTransform& UserTransform, bool bIsDefaultTransform, const FComponentInstanceDataCache* InstanceDataCache)
{
	void (*FinishSpawningInternal)(AActor*, const FTransform&, bool, const FComponentInstanceDataCache*) = decltype(FinishSpawningInternal)(ImageBase + Finder::FindAActor_FinishSpawning());
	FinishSpawningInternal(this, UserTransform, bIsDefaultTransform, InstanceDataCache);
}

bool AActor::IsNetStartupActor() const
{
	return bNetStartup;
}

bool AActor::IsRelevancyOwnerFor(const AActor* ReplicatedActor, const AActor* ActorOwner, const AActor* ConnectionActor) const
{
	bool (*&IsRelevancyOwnerForInternal)(const AActor*, const AActor*, const AActor*, const AActor*) = decltype(IsRelevancyOwnerForInternal)(VTable[Finder::FindAActor_IsRelevancyOwnerForVFT()]);
	return IsRelevancyOwnerForInternal(this, ReplicatedActor, ActorOwner, ConnectionActor);
}

bool AActor::GetNetDormancy(const FVector& ViewPos, const FVector& ViewDir, AActor* Viewer, AActor* ViewTarget, UActorChannel* InChannel, float Time, bool bLowBandwidth)
{
	// For now, per peer dormancy is not supported
	return false;
}

bool AActor::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	bool (*&IsNetRelevantForInternal)(const AActor*, const AActor*, const AActor*, const FVector&) = decltype(IsNetRelevantForInternal)(VTable[Finder::FindAActor_IsNetRelevantForVFT()]);
	return IsNetRelevantForInternal(this, RealViewer, ViewTarget, SrcLocation);
}

FVector AActor::GetActorForwardVector() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"GetActorForwardVector"));

	struct Actor_GetActorForwardVector final
	{
	public:
		FVector ReturnValue;
	};

	Actor_GetActorForwardVector Parms{};

	const_cast<AActor*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

FVector AActor::GetActorRightVector() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"GetActorRightVector"));

	struct Actor_GetActorRightVector final
	{
	public:
		FVector ReturnValue;
	};

	Actor_GetActorRightVector Parms{};

	const_cast<AActor*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

FVector AActor::GetActorUpVector() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"GetActorUpVector"));

	struct Actor_GetActorUpVector final
	{
	public:
		FVector ReturnValue;
	};

	Actor_GetActorUpVector Parms{};

	const_cast<AActor*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

FVector AActor::K2_GetActorLocation() const
{
	if (!this) {
		return FVector();
	}

	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"K2_GetActorLocation"));

	struct Actor_K2_GetActorLocation final
	{
	public:
		FVector ReturnValue;
	};

	Actor_K2_GetActorLocation Parms{};

	const_cast<AActor*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

FRotator AActor::K2_GetActorRotation() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("K2_GetActorRotation");

	struct Actor_K2_GetActorRotation final
	{
	public:
		FRotator ReturnValue;
	};

	Actor_K2_GetActorRotation Parms{};

	const_cast<AActor*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

ENetRole AActor::GetRemoteRole() const
{
	if (!this) {
		return ROLE_None;
	}

	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"GetRemoteRole"));

	if (!Func) {
		Log("AActor::GetRemoteRole: Failed to find function!");
		return ROLE_None;
	}

	struct Actor_GetRemoteRole final
	{
	public:
		ENetRole ReturnValue;
	};

	Actor_GetRemoteRole Parms{};

	const_cast<AActor*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

void AActor::CallPreReplication(UNetDriver* NetDriver)
{
	void (*CallPreReplicationInternal)(AActor*, UNetDriver*) = decltype(CallPreReplicationInternal)(ImageBase + Finder::FindAActor_CallPreReplication());
	CallPreReplicationInternal(this, NetDriver);
}

void AActor::ForceNetUpdate()
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"ForceNetUpdate"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		void (*&ForceNetUpdateInternal)(AActor*) = decltype(ForceNetUpdateInternal)(VTable[VTableIdx]);
		ForceNetUpdateInternal(this);
	}
}

const AActor* AActor::GetNetOwner() const
{
	if (IsA(APawn::StaticClass())) {
		return this;
	}
	if (IsA(APlayerController::StaticClass())) {
		return this;
	}
	if (IsA(AOnlineBeaconClient::StaticClass())) {
		AOnlineBeaconClient* BeaconClient = (AOnlineBeaconClient*)this;
		return (AActor*)BeaconClient->BeaconOwner;
	}

	return Owner;
}

void AActor::K2_DestroyActor()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"K2_DestroyActor"));

	ProcessEvent(Func, nullptr);
}

FTransform AActor::GetTransform() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetTransform");

	struct Actor_GetTransform final
	{
	public:
		FTransform ReturnValue;
	};

	Actor_GetTransform Parms{};

	const_cast<AActor*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

void AActor::SetNetDormancy(ENetDormancy NewDormancy)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("SetNetDormancy");

	struct Actor_SetNetDormancy final
	{
	public:
		ENetDormancy NewDormancy;
	};

	Actor_SetNetDormancy Parms{};

	Parms.NewDormancy = NewDormancy;

	ProcessEvent(Func, &Parms);
}

void AActor::GetActorEyesViewPoint(struct FVector* OutLocation, struct FRotator* OutRotation) const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetActorEyesViewPoint");

	struct Actor_GetActorEyesViewPoint
	{
	public:
		FVector OutLocation;
		FRotator OutRotation;
	};

	Actor_GetActorEyesViewPoint Parms{};

	const_cast<AActor*>(this)->ProcessEvent(Func, &Parms);

	if (OutLocation != nullptr)
		*OutLocation = std::move(Parms.OutLocation);

	if (OutRotation != nullptr)
		*OutRotation = std::move(Parms.OutRotation);
}