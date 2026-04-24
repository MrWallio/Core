#pragma once
#include "pch.h"

#include "UObjectBaseUtility.h"

class FConfigCacheIni;
class FEditPropertyChain;
class ITargetPlatform;
class ITransactionObjectAnnotation;
struct FFrame;
struct FObjectInstancingGraph;
struct FPropertyChangedChainEvent;
class UFunction;

class UObject : public UObjectBaseUtility {
public:
	/**
	 * Gets all default subobjects associated with this object instance.
	 * @param	OutDefaultSubobjects	Array containing all default subobjects of this object.
	 */
	void GetDefaultSubobjects(TArray<UObject*>& OutDefaultSubobjects);

	/**
	 * Finds a subobject associated with this object instance by its name
	 * @param	Name	Object name to look for
	 */
	UObject* GetDefaultSubobjectByName(FName ToFind);

	/**
	 * Called after the C++ constructor and after the properties have been initialized, including those loaded from config.
	 * This is called before any serialization or other setup has happened.
	 */
	void PostInitProperties();

	/**
	 * Do any object-specific cleanup required immediately after loading an object.
	 * This is not called for newly-created objects, and by default will always execute on the game thread.
	 */
	void PostLoad();

	/**
	 * Called before destroying the object.  This is called immediately upon deciding to destroy the object, to allow the object to begin an
	 * asynchronous cleanup process.
	 */
	void BeginDestroy();

	/**
	 * Called to check if the object is ready for FinishDestroy.  This is called after BeginDestroy to check the completion of the
	 * potentially asynchronous object cleanup.
	 * @return True if the object's asynchronous cleanup has completed and it is ready for FinishDestroy to be called.
	 */
	bool IsReadyForFinishDestroy() { return true; }

	/**
	 * Called to finish destroying the object.  After UObject::FinishDestroy is called, the object's memory should no longer be accessed.
	 *
	 * @warning Because properties are destroyed here, Super::FinishDestroy() should always be called at the end of your child class's FinishDestroy() method, rather than at the beginning.
	 */
	void FinishDestroy();

	/** Returns a UFunction with the specified name, wrapper for UClass::FindFunctionByName() */
	UFunction* FindFunction(FName InName) const;

	/** Version of FindFunction() that will assert if the function was not found */
	UFunction* FindFunctionChecked(FName InName) const;

	static UObject* GetArchetypeFromRequiredInfo(UClass* Class, UObject* Outer, FName Name, EObjectFlags ObjectFlags);

	UObject* GetArchetype() const
	{
		return GetArchetypeFromRequiredInfo(GetClass(), GetOuter(), GetFName(), ObjectFlags);
	}

	void ProcessEvent(UFunction* Function, void* Parms);
};