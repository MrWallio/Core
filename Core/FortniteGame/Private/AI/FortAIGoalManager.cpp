#include "pch.h"
#include "FortniteGame/Public/AI/FortAIGoalManager.h"

#include "FortniteGame/Public/FortGameMode/FortGameModeZone.h"
#include "Core/Public/StubCallsites.h"

AFortAIGoalManager* AFortAIGoalManager::GetCurrent(UObject* WorldContextObject) {
	if (!WorldContextObject)
		return nullptr;

	UWorld* World = WorldContextObject->GetWorld();
	if (!World)
		return nullptr;

	AFortGameModeZone* GameMode = World->AuthorityGameMode->Cast<AFortGameModeZone>();
	if (!GameMode)
		return nullptr;

	return GameMode->AIGoalManager;
}

void AFortAIGoalManager::Hook() {
	uintptr_t Stub = StubCallsites::ResolveStub(
		StubCallsites::FromString(L"OnReceiveGoalQueryResult received by AFortAIController when no Goal Manager exists!"));

	if (!Stub) {
		Log("AFortAIGoalManager::Hook: could not resolve the GetCurrent stub, no callsites patched!");
		return;
	}

	StubCallsites::Patch("AFortAIGoalManager::GetCurrent", Stub, GetCurrent, {
		{ "AFortMission::CreateEncounterAssignment", {
			StubCallsites::ByReflection("Function /Script/FortniteGame.FortMission.CreateEncounterAssignment") } },

		{ "AFortMission::AddGoalToEncounterAssignment", {
			StubCallsites::ByReflection("Function /Script/FortniteGame.FortMission.AddGoalToEncounterAssignment") } },

		{ "AFortMission::AddGoalsToEncounterAssignment", {
			StubCallsites::ByReflection("Function /Script/FortniteGame.FortMission.AddGoalsToEncounterAssignment") } },

		{ "AFortMission::FindGoalLocationsForEncounterAssignment", {
			StubCallsites::ByReflection("Function /Script/FortniteGame.FortMission.FindGoalLocationsForEncounterAssignment") } },

		{ "AFortMission::RemoveGoalFromEncounterAssignment", {
			StubCallsites::ByReflection("Function /Script/FortniteGame.FortMission.RemoveGoalFromEncounterAssignment") } },

		{ "AFortMission::RemoveGoalsFromEncounterAssignment", {
			StubCallsites::ByReflection("Function /Script/FortniteGame.FortMission.RemoveGoalsFromEncounterAssignment") } },

		{ "UFortAIEncounterInfo::CreateEncounterAssignment", {
			StubCallsites::ByReflection("Function /Script/FortniteGame.FortAIEncounterInfo.CreateEncounterAssignment") } },

		{ "AFortAIController::UnregisterForGoalDetection", {
			StubCallsites::ByReflection("Function /Script/FortniteGame.FortAIController.UnregisterForGoalDetection") } },

		{ "AFortAIDirector::IsAnyEncounterGoalWithinDistanceOfPoint", {
			StubCallsites::ByReflection("Function /Script/FortniteGame.FortAIDirector.IsAnyEncounterGoalWithinDistanceOfPoint") } },

		{ "AFortAIController::OnReceiveGoalQueryResult", {
			StubCallsites::ByString(L"OnReceiveGoalQueryResult received by AFortAIController when no Goal Manager exists!") } },

		{ "UFortAIEncounterInfo::OnEnemyPawnSpawned", {
			StubCallsites::ByString(L"Failed to find spawn group info on enemy pawn spawning for pawn %s. Associated Mission Name: %s. %s Group GUID is %s. Num active groups %i.") } },

		{ "AFortAIGoalManager::GetCurrentForAssignment", {
			StubCallsites::ByString(L"%s failed due to NULL WorldContextObject.  Without WorldContextObject, cannot find Goal Manager!  Assignment Identifier: %s.") } },

		{ "AFortAIGoalManager::GetDefaultAssignmentSettings", {
			StubCallsites::BySignature("40 57 48 83 EC 20 48 8B F9 48 85 D2 0F 85") } },

		{ "AFortAIGoalManager::GetDefaultAssignmentSettings", {
			StubCallsites::BySignature("40 57 48 83 EC 20 48 8B F9 48 85 D2 0F 85") } },

		{ "UFortAIEncounterInfo::NotifyDirectorEnd", {
			StubCallsites::BySignature("48 89 5C 24 ? 57 48 83 EC 20 48 8B 01 8B FA 48 8B D9 FF 90 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 85 C0") } },

		{ "AFortAIController::OnPawnSpawned", {
			StubCallsites::BySignature("40 55 53 56 57 41 54 48 8D 6C 24 ? 48 81 EC 00 01 00 00 45 33 E4 4C 89 B4 24") } },

		{ "UAIGoalComponent::SetActiveGoal", {
			StubCallsites::BySignature("40 53 48 83 EC 20 48 8B D9 48 85 D2 74 ? 48 39 91") } },

		{ "AFortAIGoalManager::SelectDefaultAssignmentSettings", {
			StubCallsites::BySignature("48 89 5C 24 ? 56 57 41 56 48 83 EC 30 48 8B F1") } },

		{ "AFortAIGoalManager::RemoveWorldAssignment", {
			StubCallsites::BySignature("40 53 48 83 EC 20 48 8B 01 48 8B D9 FF 90 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 85 C0 74 ? 48 8B D3 48 8B C8 48 83 C4 20 5B E9 ? ? ? ? 48 83 C4 20 5B C3 CC CC CC CC CC CC CC CC CC CC CC 48 89 5C 24"), // 1.9
			StubCallsites::BySignature("40 53 48 83 EC 20 48 8B 01 48 8B D9 FF 90 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 85 C0 74 ? 48 8B D3 48 8B C8 48 83 C4 20 5B E9 ? ? ? ? 48 83 C4 20 5B C3 90 11 C3") } }, // 3.6

		{ "AFortAIGoalManager::GetGoalLocations", {
			StubCallsites::BySignature("40 53 48 83 EC 50 48 8B D9 E8 ? ? ? ? 48 85 C0") } },

		{ "UFortQueryGenerator::GenerateItems", {
			StubCallsites::BySignature("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B DA 48 8B F1 48 85 D2 74 ? E8 ? ? ? ? 48 8B D0 48 8B CB E8 ? ? ? ? 48 8B F8 48 85 C0 74") } },

		{ "UFortQueryGenerator::GenerateItems", {
			StubCallsites::BySignature("48 89 5C 24 ? 48 89 6C 24 ? 56 48 83 EC 20 48 8B F1 48 8B DA") } },

		{ "UFortQueryTest::RunTest", {
			StubCallsites::BySignature("48 89 54 24 ? 48 89 4C 24 ? 55 53 56 57 41 56 48 8D 6C 24 ? 48 81 EC 30 01 00 00") } },

		{ "UFortQueryTest::RunTest", {
			StubCallsites::BySignature("40 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC A0 00 00 00 48 8B F1"), // 1.9
			StubCallsites::BySignature("40 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC B0 00 00 00 48 8B F1") } }, // 3.6

		{ "UFortQueryTest::RunTest", {
			StubCallsites::BySignature("48 89 5C 24 ? 55 56 57 41 56 41 57 48 8D 6C 24 ? 48 81 EC D0 00 00 00 48 8D 7A") } },

		{ "UFortQueryTest::RunTest", {
			StubCallsites::BySignature("40 55 53 56 57 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC 90 00 00 00") } },

		{ "UFortQueryTest::RunTest", {
			StubCallsites::BySignature("40 55 56 57 41 55 41 57 48 8D 6C 24 ? 48 81 EC 10 01 00 00 4C 8B F9"), // 1.9
			StubCallsites::BySignature("40 55 56 57 41 55 41 57 48 8D 6C 24 ? 48 81 EC 20 01 00 00 4C 8B F9") } }, // 3.6

		{ "UFortQueryTest::RunTest", {
			StubCallsites::BySignature("48 89 54 24 ? 48 89 4C 24 ? 55 53 56 57 41 56 48 8D 6C 24 ? 48 81 EC 50 01 00 00"), // 1.9
			StubCallsites::BySignature("48 89 54 24 ? 48 89 4C 24 ? 55 53 56 57 41 56 48 8D 6C 24 ? 48 81 EC 60 01 00 00") } }, // 3.6

		{ "UFortQueryTest::RunTest", {
			StubCallsites::BySignature("40 55 53 56 57 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC F0 00 00 00 48 8D 7A") } },
	});

	Log("AFortAIGoalManager Hooked");
}
