#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"

enum EClassCastFlags : uint64
{
	RF_NoFlags = 0x00000000,
	CASTCLASS_None = 0x0000000000000000,

	CASTCLASS_UField = 0x0000000000000001,
	CASTCLASS_FInt8Property = 0x0000000000000002,
	CASTCLASS_UEnum = 0x0000000000000004,
	CASTCLASS_UStruct = 0x0000000000000008,
	CASTCLASS_UScriptStruct = 0x0000000000000010,
	CASTCLASS_UClass = 0x0000000000000020,
	CASTCLASS_FByteProperty = 0x0000000000000040,
	CASTCLASS_FIntProperty = 0x0000000000000080,
	CASTCLASS_FFloatProperty = 0x0000000000000100,
	CASTCLASS_FUInt64Property = 0x0000000000000200,
	CASTCLASS_FClassProperty = 0x0000000000000400,
	CASTCLASS_FUInt32Property = 0x0000000000000800,
	CASTCLASS_FInterfaceProperty = 0x0000000000001000,
	CASTCLASS_FNameProperty = 0x0000000000002000,
	CASTCLASS_FStrProperty = 0x0000000000004000,
	CASTCLASS_FProperty = 0x0000000000008000,
	CASTCLASS_FObjectProperty = 0x0000000000010000,
	CASTCLASS_FBoolProperty = 0x0000000000020000,
	CASTCLASS_FUInt16Property = 0x0000000000040000,
	CASTCLASS_UFunction = 0x0000000000080000,
	CASTCLASS_FStructProperty = 0x0000000000100000,
	CASTCLASS_FArrayProperty = 0x0000000000200000,
	CASTCLASS_FInt64Property = 0x0000000000400000,
	CASTCLASS_FDelegateProperty = 0x0000000000800000,
	CASTCLASS_FNumericProperty = 0x0000000001000000,
	CASTCLASS_FMulticastDelegateProperty = 0x0000000002000000,
	CASTCLASS_FObjectPropertyBase = 0x0000000004000000,
	CASTCLASS_FWeakObjectProperty = 0x0000000008000000,
	CASTCLASS_FLazyObjectProperty = 0x0000000010000000,
	CASTCLASS_FSoftObjectProperty = 0x0000000020000000,
	CASTCLASS_FTextProperty = 0x0000000040000000,
	CASTCLASS_FInt16Property = 0x0000000080000000,
	CASTCLASS_FDoubleProperty = 0x0000000100000000,
	CASTCLASS_FSoftClassProperty = 0x0000000200000000,
	CASTCLASS_UPackage = 0x0000000400000000,
	CASTCLASS_ULevel = 0x0000000800000000,
	CASTCLASS_AActor = 0x0000001000000000,
	CASTCLASS_APlayerController = 0x0000002000000000,
	CASTCLASS_APawn = 0x0000004000000000,
	CASTCLASS_USceneComponent = 0x0000008000000000,
	CASTCLASS_UPrimitiveComponent = 0x0000010000000000,
	CASTCLASS_USkinnedMeshComponent = 0x0000020000000000,
	CASTCLASS_USkeletalMeshComponent = 0x0000040000000000,
	CASTCLASS_UBlueprint = 0x0000080000000000,
	CASTCLASS_UDelegateFunction = 0x0000100000000000,
	CASTCLASS_UStaticMeshComponent = 0x0000200000000000,
	CASTCLASS_FMapProperty = 0x0000400000000000,
	CASTCLASS_FSetProperty = 0x0000800000000000,
	CASTCLASS_FEnumProperty = 0x0001000000000000,
	CASTCLASS_USparseDelegateFunction = 0x0002000000000000,
	CASTCLASS_FMulticastInlineDelegateProperty = 0x0004000000000000,
	CASTCLASS_FMulticastSparseDelegateProperty = 0x0008000000000000,
	CASTCLASS_FFieldPathProperty = 0x0010000000000000,

	CASTCLASS_UStructProperty = CASTCLASS_FStructProperty,
	CASTCLASS_UObjectProperty = CASTCLASS_FObjectProperty,
	CASTCLASS_UArrayProperty = CASTCLASS_FArrayProperty,
	CASTCLASS_UBoolProperty = CASTCLASS_FBoolProperty,
	CASTCLASS_UByteProperty = CASTCLASS_FByteProperty,
	CASTCLASS_UEnumProperty = CASTCLASS_FEnumProperty,
	CASTCLASS_UNameProperty = CASTCLASS_FNameProperty,
	CASTCLASS_UStrProperty = CASTCLASS_FStrProperty,
	CASTCLASS_UClassProperty = CASTCLASS_FClassProperty,
	CASTCLASS_UMapProperty = CASTCLASS_FMapProperty,
	CASTCLASS_USetProperty = CASTCLASS_FSetProperty,
	CASTCLASS_UProperty = CASTCLASS_FProperty,
};

enum EPropertyFlags : uint64
{
	CPF_None = 0,

	CPF_Edit = 0x0000000000000001,
	CPF_ConstParm = 0x0000000000000002,
	CPF_BlueprintVisible = 0x0000000000000004,
	CPF_ExportObject = 0x0000000000000008,
	CPF_BlueprintReadOnly = 0x0000000000000010,
	CPF_Net = 0x0000000000000020,
	CPF_EditFixedSize = 0x0000000000000040,
	CPF_Parm = 0x0000000000000080,
	CPF_OutParm = 0x0000000000000100,
	CPF_ZeroConstructor = 0x0000000000000200,
	CPF_ReturnParm = 0x0000000000000400,
	CPF_DisableEditOnTemplate = 0x0000000000000800,
	CPF_Transient = 0x0000000000002000,
	CPF_Config = 0x0000000000004000,
	CPF_DisableEditOnInstance = 0x0000000000010000,
	CPF_EditConst = 0x0000000000020000,
	CPF_GlobalConfig = 0x0000000000040000,
	CPF_InstancedReference = 0x0000000000080000,
	CPF_DuplicateTransient = 0x0000000000200000,
	CPF_SaveGame = 0x0000000001000000,
	CPF_NoClear = 0x0000000002000000,
	CPF_ReferenceParm = 0x0000000008000000,
	CPF_BlueprintAssignable = 0x0000000010000000,
	CPF_Deprecated = 0x0000000020000000,
	CPF_IsPlainOldData = 0x0000000040000000,
	CPF_RepSkip = 0x0000000080000000,
	CPF_RepNotify = 0x0000000100000000,
	CPF_Interp = 0x0000000200000000,
	CPF_NonTransactional = 0x0000000400000000,
	CPF_EditorOnly = 0x0000000800000000,
	CPF_NoDestructor = 0x0000001000000000,
	CPF_AutoWeak = 0x0000004000000000,
	CPF_ContainsInstancedReference = 0x0000008000000000,
	CPF_AssetRegistrySearchable = 0x0000010000000000,
	CPF_SimpleDisplay = 0x0000020000000000,
	CPF_AdvancedDisplay = 0x0000040000000000,
	CPF_Protected = 0x0000080000000000,
	CPF_BlueprintCallable = 0x0000100000000000,
	CPF_BlueprintAuthorityOnly = 0x0000200000000000,
	CPF_TextExportTransient = 0x0000400000000000,
	CPF_NonPIEDuplicateTransient = 0x0000800000000000,
	CPF_ExposeOnSpawn = 0x0001000000000000,
	CPF_PersistentInstance = 0x0002000000000000,
	CPF_UObjectWrapper = 0x0004000000000000,
	CPF_HasGetValueTypeHash = 0x0008000000000000,
	CPF_NativeAccessSpecifierPublic = 0x0010000000000000,
	CPF_NativeAccessSpecifierProtected = 0x0020000000000000,
	CPF_NativeAccessSpecifierPrivate = 0x0040000000000000,
	CPF_SkipSerialization = 0x0080000000000000,
};

inline EPropertyFlags operator|(EPropertyFlags A, EPropertyFlags B) { return (EPropertyFlags)((uint64)A | (uint64)B); }
inline EClassCastFlags operator|(EClassCastFlags A, EClassCastFlags B) { return (EClassCastFlags)((uint64)A | (uint64)B); }

enum EObjectFlags : uint32
{
	RF_Public = 0x00000001,
	RF_Standalone = 0x00000002,
	RF_MarkAsNative = 0x00000004,
	RF_Transactional = 0x00000008,
	RF_ClassDefaultObject = 0x00000010,
	RF_ArchetypeObject = 0x00000020,
	RF_Transient = 0x00000040,

	RF_MarkAsRootSet = 0x00000080,
	RF_TagGarbageTemp = 0x00000100,

	RF_NeedInitialization = 0x00000200,
	RF_NeedLoad = 0x00000400,
	RF_KeepForCooker = 0x00000800,
	RF_NeedPostLoad = 0x00001000,
	RF_NeedPostLoadSubobjects = 0x00002000,
	RF_NewerVersionExists = 0x00004000,
	RF_BeginDestroyed = 0x00008000,
	RF_FinishDestroyed = 0x00010000,

	RF_BeingRegenerated = 0x00020000,
	RF_DefaultSubObject = 0x00040000,
	RF_WasLoaded = 0x00080000,
	RF_TextExportTransient = 0x00100000,
	RF_LoadCompleted = 0x00200000,
	RF_InheritableComponentTemplate = 0x00400000,
	RF_DuplicateTransient = 0x00800000,
	RF_StrongRefOnFrame = 0x01000000,
	RF_NonPIEDuplicateTransient = 0x02000000,
	RF_Dynamic = 0x04000000,
	RF_WillBeLoaded = 0x08000000,
	RF_HasExternalPackage = 0x10000000,
};

inline EObjectFlags operator|(EObjectFlags A, EObjectFlags B) { return (EObjectFlags)((uint32)A | (uint32)B); }
inline EObjectFlags operator&(EObjectFlags A, EObjectFlags B) { return (EObjectFlags)((uint32)A & (uint32)B); }
inline EObjectFlags operator~(EObjectFlags A) { return (EObjectFlags)(~(uint32)A); }
inline EObjectFlags& operator|=(EObjectFlags& A, EObjectFlags B) { A = A | B; return A; }
inline EObjectFlags& operator&=(EObjectFlags& A, EObjectFlags B) { A = A & B; return A; }

enum class EInternalObjectFlags : int32
{
	None = 0,

	ReachableInCluster = 1 << 23,
	ClusterRoot = 1 << 24,
	Native = 1 << 25,
	Async = 1 << 26,
	AsyncLoading = 1 << 27,
	Unreachable = 1 << 28,
	PendingKill = 1 << 29,
	RootSet = 1 << 30,
};

#define DefineCustomProperty(PropertyType, PropertyName, Offset) \
public: \
    FORCEINLINE PropertyType& _Get##PropertyName() { \
        return *reinterpret_cast<PropertyType*>((uintptr_t)this + Offset); \
    } \
    FORCEINLINE PropertyType _Get##PropertyName() const { \
        return *reinterpret_cast<PropertyType*>((uintptr_t)this + Offset); \
    } \
    FORCEINLINE void _Set##PropertyName(PropertyType Value) { \
        *reinterpret_cast<PropertyType*>((uintptr_t)this + Offset) = Value; \
    } \
public: \
    __declspec(property(get = _Get##PropertyName, put = _Set##PropertyName)) PropertyType PropertyName;