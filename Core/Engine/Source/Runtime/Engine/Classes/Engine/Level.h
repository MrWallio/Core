#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Core/Public/Math/IntVector.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/WeakObjectPtr.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/EngineBaseTypes.h"
#include "Engine/Source/Runtime/Core/Public/Misc/Guid.h"

class UWorld;
class AActor;
class ABrush;
class AInstancedFoliageActor;
class ALevelBounds;
class APlayerController;
class AWorldSettings;
class FSceneInterface;
class ITargetPlatform;
class UAssetUserData;
class UMapBuildDataRegistry;
class UNavigationDataChunk;
class UTexture2D;
struct FLevelCollection;
class ULevelActorContainer;
class FLevelPartitionOperationScope;
class FRegisterComponentContext;
class ALevelScriptActor;
struct FVector;
class UModel;
class UModelComponent;
class ANavigationObjectBase;
class FTickTaskLevel;
class FPrecomputedLightVolume;

class ULevel : public UObject {
public:
	DefineUnrealClass(ULevel);

	/** Array of all actors in this level, used by FActorIteratorBase and derived classes */
	DefineUProperty(TArray<AActor*>, Actors);

	/**
	 * The World that has this level in its Levels array.
	 * This is not the same as GetOuter(), because GetOuter() for a streaming level is a vestigial world that is not used.
	 * It should not be accessed during BeginDestroy(), just like any other UObject references, since GC may occur in any order.
	 */
	DefineUProperty(UWorld*, OwningWorld);

	/** Whether the level is currently visible/ associated with the world */
	DefineBitfieldUProperty(bIsVisible);

	/** The level scripting actor, created by instantiating the class from LevelScriptBlueprint.  This handles all level scripting */
	DefineUProperty(ALevelScriptActor*, LevelScriptActor);

	DefineUProperty(AWorldSettings*, WorldSettings);

	DefineBitfieldUProperty(bLocked);
public:
	bool IsPersistentLevel() const;

	bool HasVisibilityChangeRequestPending() const;

	class ALevelScriptActor* GetLevelScriptActor() const;

	bool HasVisibilityRequestPending() const;

	bool IsCurrentLevel() const;
};

class ULevelUE416 : public ULevel {
public:
	/** URL associated with this level. */
	FURLUE416 URL;

	uint8 Padding1[0x8];

	/** Array of all actors in this level, used by FActorIteratorBase and derived classes */
	TArray<AActor*> Actors;

	/** Array of actors to be exposed to GC in this level. All other actors will be referenced through ULevelActorContainer */
	TArray<AActor*> ActorsForGC;

	/**
	 * The World that has this level in its Levels array.
	 * This is not the same as GetOuter(), because GetOuter() for a streaming level is a vestigial world that is not used.
	 * It should not be accessed during BeginDestroy(), just like any other UObject references, since GC may occur in any order.
	 */
	UWorld* OwningWorld;

	/** BSP UModel. */
	UModel* Model;

	/** BSP Model components used for rendering. */
	TArray<UModelComponent*> ModelComponents;

	ULevelActorContainer* ActorCluster;

	/** Num of components missing valid texture streaming data. Updated in map check. */
	int32 NumTextureStreamingUnbuiltComponents;

	/** Num of resources that have changed since the last texture streaming build. Updated in map check. */
	int32 NumTextureStreamingDirtyResources;

	/** The level scripting actor, created by instantiating the class from LevelScriptBlueprint.  This handles all level scripting */
	ALevelScriptActor* LevelScriptActor;

	/**
	 * Start and end of the navigation list for this level, used for quickly fixing up
	 * when streaming this level in/out. @TODO DEPRECATED - DELETE
	 */
	ANavigationObjectBase* NavListStart;
	ANavigationObjectBase* NavListEnd;

	/** Navigation related data that can be stored per level */
	TArray<UNavigationDataChunk*> NavDataChunks;

	/** Total number of KB used for lightmap textures in the level. */
	float LightmapTotalSize;
	/** Total number of KB used for shadowmap textures in the level. */
	float ShadowmapTotalSize;

	/** threes of triangle vertices - AABB filtering friendly. Stored if there's a runtime need to rebuild navigation that accepts BSPs
	 *	as well - it's a lot easier this way than retrieve this data at runtime */
	TArray<FVector> StaticNavigableGeometry;

	/** The Guid of each texture refered by FStreamingTextureBuildInfo::TextureLevelIndex	*/
	TArray<FGuid> StreamingTextureGuids;

	/** Data structures for holding the tick functions **/
	FTickTaskLevel* TickTaskLevel;

	/**
	* The precomputed light information for this level.
	* The extra level of indirection is to allow forward declaring FPrecomputedLightVolume.
	*/
	FPrecomputedLightVolume* PrecomputedLightVolume;

	uint8 Padding[0x78];

	/**
	 * Whether the level is a lighting scenario.  Lighting is built separately for each lighting scenario level with all other scenario levels hidden.
	 * Only one lighting scenario level should be visible at a time for correct rendering, and lightmaps from that level will be used on the rest of the world.
	 * Note: When a lighting scenario level is present, lightmaps for all streaming levels are placed in the scenario's _BuildData package.
	 *		This means that lightmaps for those streaming levels will not be streamed with them.
	 */
	bool bIsLightingScenario;

	uint8 _pad1[0x3];

	/** Identifies map build data specific to this level, eg lighting volume samples. */
	FGuid LevelBuildDataId;

	uint8 _pad2[0x4];

	/** Registry for data from the map build.  This is stored in a separate package from the level to speed up saving / autosaving. */
	UMapBuildDataRegistry* MapBuildData;

	/** Level offset at time when lighting was built */
	FIntVector LightBuildLevelOffset;

	/** Whether components are currently registered or not. */
	uint8										bAreComponentsCurrentlyRegistered : 1;

	/** Whether the geometry needs to be rebuilt for correct lighting */
	uint8										bGeometryDirtyForLighting : 1;

	/** Whether a level transform rotation was applied since the texture streaming builds. Invalidates the precomputed streaming bounds. */
	uint8 										bTextureStreamingRotationChanged : 1;

	/** Whether the level is currently visible/ associated with the world */
	uint8										bIsVisible : 1;

	/** Whether this level is locked; that is, its actors are read-only
	 *	Used by WorldBrowser to lock a level when corresponding ULevelStreaming does not exist
	 */
	uint8 										bLocked : 1;

	/** The below variables are used temporarily while making a level visible.				*/

	/** Whether we already moved actors.													*/
	uint8										bAlreadyMovedActors : 1;
	/** Whether we already shift actors positions according to world composition.			*/
	uint8										bAlreadyShiftedActors : 1;
	/** Whether we already updated components.												*/
	uint8										bAlreadyUpdatedComponents : 1;
	/** Whether we already associated streamable resources.									*/
	uint8										bAlreadyAssociatedStreamableResources : 1;
	/** Whether we already initialized network actors.											*/
	uint8										bAlreadyInitializedNetworkActors : 1;
	/** Whether we already routed initialize on actors.										*/
	uint8										bAlreadyRoutedActorInitialize : 1;
	/** Whether we already sorted the actor list.											*/
	uint8										bAlreadySortedActorList : 1;
	/** Whether this level is in the process of being associated with its world				*/
	uint8										bIsAssociatingLevel : 1;
	/** Whether this level should be fully added to the world before rendering his components	*/
	uint8										bRequireFullVisibilityToRender : 1;
	/** Whether this level is specific to client, visibility state will not be replicated to server	*/
	uint8										bClientOnlyVisible : 1;
	/** Whether this level was duplicated for PIE	*/
	uint8										bWasDuplicatedForPIE : 1;
	/** Whether the level is currently being removed from the world */
	uint8										bIsBeingRemoved : 1;
	/** Whether this level has gone through a complete rerun construction script pass. */
	uint8										bHasRerunConstructionScripts : 1;
	/** Whether the level had its actor cluster created. This doesn't mean that the creation was successful. */
	uint8										bActorClusterCreated : 1;

	uint8 _unusedBits : 5;

	uint8 _pad3[0x1];

	/** Current index into actors array for updating components.							*/
	int32										CurrentActorIndexForUpdateComponents;
	/** Current index into actors array for updating components.							*/
	int32										CurrentActorIndexForUnregisterComponents;

	uint8 Padding2[0xF0];

	AWorldSettings* WorldSettings;

	/** Cached level collection that this level is contained in, for faster access than looping through the collections in the world. */
	FLevelCollection* CachedLevelCollection;

	/** Array of user data stored with the asset */
	TArray<UAssetUserData*> AssetUserData;

	uint8 Padding4[0x10];
};