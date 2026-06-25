#include "pch.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateAthena.h"

void AFortPlayerStateAthena::OnRep_TeamKillScore()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_TeamKillScore");

	if (!Func) {
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
		return;
	}

	ProcessEvent(Func, nullptr);
}

void AFortPlayerStateAthena::ClientReportDBNO(const AFortPlayerStateAthena* Player)
{
	if (Version::Fortnite_Version <= 1.91 && Version::Fortnite_Version != 1.10 && Version::Fortnite_Version != 1.11) {
		ClientReportDBNO(Player->GetPlayerName());
	}
	else {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = FindFunction("ClientReportDBNO");

		if (!Func) {
			return;
		}

		struct FortPlayerStateAthena_ClientReportDBNO
		{
		public:
			const AFortPlayerStateAthena* Player;
		};

		FortPlayerStateAthena_ClientReportDBNO Parms{};

		Parms.Player = Player;

		ProcessEvent(Func, &Parms);
	}
}

void AFortPlayerStateAthena::ClientReportDBNO(const FString& DBNOPlayersName)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientReportDBNO");

	if (!Func) {
		return;
	}

	struct FortPlayerStateAthena_ClientReportDBNO
	{
	public:
		FString DBNOPlayersName;
	};

	FortPlayerStateAthena_ClientReportDBNO Parms{};

	Parms.DBNOPlayersName = std::move(DBNOPlayersName);

	ProcessEvent(Func, &Parms);
}

void AFortPlayerStateAthena::ClientReportKill(const AFortPlayerStateAthena* Player)
{
	if (Version::Fortnite_Version <= 1.91 && Version::Fortnite_Version != 1.10 && Version::Fortnite_Version != 1.11) {
		ClientReportKill(Player->GetPlayerName());
	}
	else {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = FindFunction("ClientReportKill");

		if (!Func) {
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
		return;
	}

	ProcessEvent(Func, nullptr);
}

void AFortPlayerStateAthena::OnRep_DeathInfo()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_DeathInfo");

	if (!Func) {
		return;
	}

	ProcessEvent(Func, nullptr);
}

void AFortPlayerStateAthena::OnRep_Downs()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_Downs");

	if (!Func) {
		return;
	}

	ProcessEvent(Func, nullptr);
}

void AFortPlayerStateAthena::OnRep_Place()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_Place");

	if (!Func) {
		return;
	}

	ProcessEvent(Func, nullptr);
}

void AFortPlayerStateAthena::OnRep_TeamIndex()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_TeamIndex");

	if (!Func) {
		return;
	}

	ProcessEvent(Func, nullptr);
}

uint8 AFortPlayerStateAthena::ToDeathCause(const FGameplayTagContainer& InTags, bool bWasDBNO)
{
	if (Version::Fortnite_Version <= 3.0 || Version::Fortnite_Version == 1.10 || Version::Fortnite_Version == 1.11) {
		// need to reimplement this for older versions, but for now just return unspecified
		return EDeathCause::GetUnspecified();
	}
	else {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerStateAthena:ToDeathCause");

		struct FortPlayerStateAthena_ToDeathCause
		{
		public:
			FGameplayTagContainer InTags;
			bool bWasDBNO;
			uint8 ReturnValue;
		};

		FortPlayerStateAthena_ToDeathCause Parms{};

		Parms.InTags = std::move(InTags);
		Parms.bWasDBNO = bWasDBNO;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		return Parms.ReturnValue;
	}
}