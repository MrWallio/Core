#include "pch.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateAthena.h"

void AFortPlayerStateAthena::OnRep_TeamKillScore()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_TeamKillScore");

	if (!Func) {
		Log("OnRep_TeamKillScore: Function not found!");
		return;
	}

	ProcessEvent(Func, nullptr);
}

void AFortPlayerStateAthena::OnRep_TeamScore()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_TeamScore");

	if (!Func) {
		Log("OnRep_TeamScore: Function not found!");
		return;
	}

	ProcessEvent(Func, nullptr);
}

void AFortPlayerStateAthena::ClientReportKill(const AFortPlayerStateAthena* Player)
{
	if (Version::Fortnite_Version > 1.91 || Version::Fortnite_Version == 1.10 || Version::Fortnite_Version == 1.11) {
		ClientReportKill(Player->PlayerName);
	}
	else {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = FindFunction("ClientReportKill");

		if (!Func) {
			Log("ClientReportKill: Function not found!");
			return;
		}

		struct FortPlayerStateAthena_ClientReportKill
		{
		public:
			const AFortPlayerStateAthena* Player;
		};

		FortPlayerStateAthena_ClientReportKill Parms{};

		Parms.Player = Player;

		ProcessEvent(Func, &Parms);
	}
}

void AFortPlayerStateAthena::ClientReportKill(const FString& KilledPlayersName)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientReportKill");

	if (!Func) {
		Log("ClientReportKill: Function not found!");
		return;
	}

	struct FortPlayerStateAthena_ClientReportKill
	{
	public:
		FString KilledPlayersName;
	};

	FortPlayerStateAthena_ClientReportKill Parms{};

	Parms.KilledPlayersName = std::move(KilledPlayersName);

	ProcessEvent(Func, &Parms);
}

void AFortPlayerStateAthena::ClientReportTeamKill(int32 TeamKills)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientReportTeamKill");

	if (!Func) {
		Log("ClientReportTeamKill: Function not found!");
		return;
	}

	struct FortPlayerStateAthena_ClientReportTeamKill
	{
	public:
		int32 TeamKills;
	};

	FortPlayerStateAthena_ClientReportTeamKill Parms{};

	Parms.TeamKills = TeamKills;

	ProcessEvent(Func, &Parms);
}

void AFortPlayerStateAthena::OnRep_Kills()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_Kills");

	if (!Func) {
		Log("OnRep_Kills: Function not found!");
		return;
	}

	ProcessEvent(Func, nullptr);
}