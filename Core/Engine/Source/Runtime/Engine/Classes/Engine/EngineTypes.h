#pragma once
#include "pch.h"

class AActor;
class UDecalComponent;
class UPhysicalMaterial;
class UPhysicalMaterialMask;
class UPrimitiveComponent;
class USceneComponent;

/** Indicates the type of a level collection, used in FLevelCollection. */
enum class ELevelCollectionType : uint8
{
	/**
	 * The dynamic levels that are used for normal gameplay and the source for any duplicated collections.
	 * Will contain a world's persistent level and any streaming levels that contain dynamic or replicated gameplay actors.
	 */
	DynamicSourceLevels,

	/** Gameplay relevant levels that have been duplicated from DynamicSourceLevels if requested by the game. */
	DynamicDuplicatedLevels,

	/**
	 * These levels are shared between the source levels and the duplicated levels, and should contain
	 * only static geometry and other visuals that are not replicated or affected by gameplay.
	 * These will not be duplicated in order to save memory.
	 */
	StaticLevels,

	MAX
};

/** Defines available strategies for handling the case where an actor is spawned in such a way that it penetrates blocking collision. */
enum class ESpawnActorCollisionHandlingMethod : uint8
{
	/** Fall back to default settings. */
	Undefined,
	/** Actor will spawn in desired location, regardless of collisions. */
	AlwaysSpawn,
	/** Actor will try to find a nearby non-colliding location (based on shape components), but will always spawn even if one cannot be found. */
	AdjustIfPossibleButAlwaysSpawn,
	/** Actor will try to find a nearby non-colliding location (based on shape components), but will NOT spawn unless one is found. */
	AdjustIfPossibleButDontSpawnIfColliding,
	/** Actor will fail to spawn. */
	DontSpawnIfColliding,
};

/** Describes if an actor can enter a low network bandwidth dormant mode */
enum ENetDormancy
{
	/** This actor can never go network dormant. */
	DORM_Never,
	/** This actor can go dormant, but is not currently dormant. Game code will tell it when it go dormant. */
	DORM_Awake,
	/** This actor wants to go fully dormant for all connections. */
	DORM_DormantAll,
	/** This actor may want to go dormant for some connections, GetNetDormancy() will be called to find out which. */
	DORM_DormantPartial,
	/** This actor is initially dormant for all connection if it was placed in map. */
	DORM_Initial,

	DORM_MAX,
};

namespace EWorldType
{
	enum Type
	{
		None,
		Game,
		Editor,
		PIE,
		EditorPreview,
		GamePreview,
		GameRPC,
		Inactive,
	};
}

enum ENetRole
{
	/** No role at all. */
	ROLE_None,
	/** Locally simulated proxy of this actor. */
	ROLE_SimulatedProxy,
	/** Locally autonomous proxy of this actor. */
	ROLE_AutonomousProxy,
	/** Authoritative control over the actor. */
	ROLE_Authority,
	ROLE_MAX,
};

enum EMovementMode
{
	MOVE_None,
	MOVE_Walking,
	MOVE_NavWalking,
	MOVE_Falling,
	MOVE_Swimming,
	MOVE_Flying,
	MOVE_Custom,
	MOVE_MAX,
};

namespace EComponentMobility
{
	enum Type
	{
		Static,
		Stationary,
		Movable,
	};
}

namespace ECollisionEnabled
{
	enum Type
	{
		NoCollision,
		QueryOnly,
		PhysicsOnly,
		QueryAndPhysics,
	};
}