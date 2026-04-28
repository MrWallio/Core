#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

namespace ENetworkLagState
{
	enum Type
	{
		/** The net driver is operating normally or it is not possible to tell if it is lagging */
		NotLagging,
		/** The net driver is in the process of timing out all of the client connections */
		Lagging
	};
}

/**
 * The network mode the game is currently running.
 * @see https://docs.unrealengine.com/latest/INT/Gameplay/Networking/Overview/
 */
enum ENetMode
{
	/** Standalone: a game without networking, with one or more local players. Still considered a server because it has all server functionality. */
	NM_Standalone,

	/** Dedicated server: server with no local players. */
	NM_DedicatedServer,

	/** Listen server: a server that also has a local player who is hosting the game, available to other players on the network. */
	NM_ListenServer,

	/**
	 * Network client: client connected to a remote server.
	 * Note that every mode less than this value is a kind of server, so checking NetMode < NM_Client is always some variety of server.
	 */
	NM_Client,

	NM_MAX,
};

struct FURL
{
public:
	DefineUnrealStruct(FURL);
public:
	// Protocol, i.e. "unreal" or "http".
	FString Protocol;

	// Optional hostname, i.e. "204.157.115.40" or "unreal.epicgames.com", blank if local.
	FString Host;

	// Optional host port.
	int32 Port;

	int32 Valid;

	// Map name, i.e. "SkyCity", default is "Entry".
	FString Map;

	// Optional place to download Map if client does not possess it
	FString RedirectURL;

	// Options.
	TArray<FString> Op;

	// Portal to enter through, default is "".
	FString Portal;

	/**
	 * Returns whether this URL corresponds to an internal object, i.e. an Unreal
	 * level which this app can try to connect to locally or on the net. If this
	 * is false, the URL refers to an object that a remote application like Internet
	 * Explorer can execute.
	 */
	bool IsInternal() const;

	/**
	 * Returns whether this URL corresponds to an internal object on this local
	 * process. In this case, no Internet use is necessary.
	 */
	bool IsLocalInternal() const;

	/**
	 * Tests if the URL contains an option string.
	 */
	bool HasOption(const TCHAR* Test) const;

	/**
	 * Returns the value associated with an option.
	 *
	 * @param Match The name of the option to get.
	 * @param Default The value to return if the option wasn't found.
	 *
	 * @return The value of the named option, or Default if the option wasn't found.
	 */
	const TCHAR* GetOption(const TCHAR* Match, const TCHAR* Default) const;

	/**
	 * Add a unique option to the URL, replacing any existing one.
	 */
	void AddOption(const TCHAR* Str);

	/**
	 * Remove an option from the URL
	 */
	void RemoveOption(const TCHAR* Key, const TCHAR* Section = nullptr, const FString& Filename = FString());

	/**
	 * Convert this URL to text.
	 */
	FString ToString(bool FullyQualified = 0) const;

	/**
	 * Prepares the Host and Port values into a standards compliant string
	 */
	FString GetHostPortString() const;
};

struct FURLUE416
{
public:
	FString Protocol;
	FString Host;
	int32 Port;
	uint8 Padding1[0x4];
	FString Map;
	FString RedirectUrl;
	TArray<FString> Op;
	FString Portal;
	int32 Valid;
	uint8 Padding2[0x4];
};