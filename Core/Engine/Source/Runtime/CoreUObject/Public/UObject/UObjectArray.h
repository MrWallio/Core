#pragma once
#include "pch.h"
#include "Core/Public/Version.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "ObjectMacros.h"
#include "UObjectBase.h"
#include "Engine/Source/Runtime/Core/Public/Misc/AssertionMacros.h"

class UObject;
class UClass;
class UStruct;

struct FUObjectItem
{
	// Pointer to the allocated object
	UObjectBase* Object;
	// Internal flags
	int32 Flags;
	// UObject Owner Cluster Index
	int32 ClusterRootIndex;
	// Weak Object Pointer Serial number associated with the object
	int32 SerialNumber;

	FORCEINLINE void SetOwnerIndex(int32 OwnerIndex)
	{
		ClusterRootIndex = OwnerIndex;
	}

	FORCEINLINE int32 GetOwnerIndex() const
	{
		return ClusterRootIndex;
	}

	FORCEINLINE void SetClusterIndex(int32 ClusterIndex)
	{
		ClusterRootIndex = -ClusterIndex - 1;
	}

	FORCEINLINE int32 GetClusterIndex() const
	{
		checkSlow(ClusterRootIndex < 0);
		return -ClusterRootIndex - 1;
	}

	FORCEINLINE int32 GetSerialNumber() const
	{
		return SerialNumber;
	}

	FORCEINLINE void ResetSerialNumberAndFlags()
	{
		Flags = 0;
		ClusterRootIndex = 0;
		SerialNumber = 0;
	}
};

class FFixedUObjectArray
{
public:
	/** Static master table to chunks of pointers **/
	FUObjectItem* Objects;
	/** Number of elements we currently have **/
	int MaxElements;
	/** Current number of UObject slots */
	int NumElements;
public:
	void PreAllocate(int32 InMaxElements)
	{
		check(!Objects);
		Objects = new FUObjectItem[InMaxElements];
		MaxElements = InMaxElements;
	}

	int32 AddSingle()
	{
		int32 Result = NumElements;
		check(Result == NumElements);
		++NumElements;
		check(Objects[Result].Object == nullptr);
		return Result;
	}

	int32 AddRange(int32 Count)
	{
		int32 Result = NumElements + Count - 1;
		check(Result == (NumElements + Count - 1));
		NumElements += Count;
		check(Objects[Result].Object == nullptr);
		return Result;
	}

	FUObjectItem const* GetObjectPtr(int32 Index) const
	{
		return &Objects[Index];
	}

	FUObjectItem* GetObjectPtr(int32 Index)
	{
		return &Objects[Index];
	}

	FORCEINLINE int32 Num() const
	{
		return NumElements;
	}

	FORCEINLINE bool IsValidIndex(int32 Index) const
	{
		return Index < Num() && Index >= 0;
	}
	FORCEINLINE FUObjectItem const& operator[](int32 Index) const
	{
		FUObjectItem const* ItemPtr = GetObjectPtr(Index);
		check(ItemPtr);
		return *ItemPtr;
	}

	FORCEINLINE FUObjectItem& operator[](int32 Index)
	{
		FUObjectItem* ItemPtr = GetObjectPtr(Index);
		check(ItemPtr);
		return *ItemPtr;
	}
};

class FChunkedFixedUObjectArray
{
public:
	enum
	{
		NumElementsPerChunk = 0x10000,
	};

	/** Master table to chunks of pointers **/
	FUObjectItem** Objects;
	/** If requested, a contiguous memory where all objects are allocated **/
	FUObjectItem* PreAllocatedObjects;
	/** Maximum number of elements **/
	const int32 MaxElements;
	/** Number of elements we currently have **/
	const int32 NumElements;
	/** Maximum number of chunks **/
	const int32 MaxChunks;
	/** Number of chunks we currently have **/
	const int32 NumChunks;

public:
	FORCEINLINE int32 Num() const
	{
		return NumElements;
	}

	FORCEINLINE int32 Capacity() const
	{
		return MaxElements;
	}

	FORCEINLINE bool IsValidIndex(int32 Index) const
	{
		return Index < Num() && Index >= 0;
	}

	FORCEINLINE int32 GetNumElementsPerChunk() const
	{
		return MaxChunks > 0 ? (MaxElements / MaxChunks) : (int32)NumElementsPerChunk;
	}

	FUObjectItem const* GetObjectPtr(int32 Index) const
	{
		const int32 PerChunk = GetNumElementsPerChunk();
		const int32 ChunkIndex = Index / PerChunk;
		const int32 WithinChunkIndex = Index % PerChunk;
		FUObjectItem* Chunk = Objects[ChunkIndex];
		return Chunk + WithinChunkIndex;
	}
	FUObjectItem* GetObjectPtr(int32 Index)
	{
		if (Index < 0 || Index > Capacity())
			return nullptr;

		const int32 PerChunk = GetNumElementsPerChunk();
		const int32 ChunkIndex = Index / PerChunk;
		const int32 WithinChunkIndex = Index % PerChunk;

		return Objects[ChunkIndex] + WithinChunkIndex;
	}

	FORCEINLINE FUObjectItem const& operator[](int32 Index) const
	{
		FUObjectItem const* ItemPtr = GetObjectPtr(Index);
		return *ItemPtr;
	}
	FORCEINLINE FUObjectItem& operator[](int32 Index)
	{
		FUObjectItem* ItemPtr = GetObjectPtr(Index);
		return *ItemPtr;
	}

	FUObjectItem*** GetRootBlockForDebuggerVisualizers()
	{
		return nullptr;
	}

	int64 GetAllocatedSize() const
	{
		return MaxChunks * sizeof(FUObjectItem*) + NumChunks * NumElementsPerChunk * sizeof(FUObjectItem);
	}
};

class FUObjectArray
{
public:
	static inline FUObjectItem* IndexToObject(int32 Index)
	{
		if (!Finder::FindGUObjectArray()) return 0;

		if (Version::Engine_Version >= 4.21)
		{
			FChunkedFixedUObjectArray* ChunkedArray = (FChunkedFixedUObjectArray*)(ImageBase + Finder::FindGUObjectArray());
			return ChunkedArray->GetObjectPtr(Index);
		}
		else
		{
			FFixedUObjectArray* FixedArray = (FFixedUObjectArray*)(ImageBase + Finder::FindGUObjectArray());
			return FixedArray->GetObjectPtr(Index);
		}
	}

	static inline int32 Num() {
		if (!Finder::FindGUObjectArray()) return 0;

		if (Version::Engine_Version >= 4.21)
		{
			FChunkedFixedUObjectArray* ChunkedArray = (FChunkedFixedUObjectArray*)(ImageBase + Finder::FindGUObjectArray());
			return ChunkedArray->Num();
		}
		else
		{
			FFixedUObjectArray* FixedArray = (FFixedUObjectArray*)(ImageBase + Finder::FindGUObjectArray());
			return FixedArray->Num();
		}
	}

	static UObject* FindObject(const std::string& FullName, bool bStrictCheck = true, EClassCastFlags Flags = EClassCastFlags::RF_NoFlags);
	static UObject* FindObjectWithClass(const std::string& FullName, bool bStrictCheck = true, UClass* Class = nullptr);

	static UObject* FindObjectFast(FName Name);
	static UObject* FindObjectFast(const char* Name);
	static UObject* FindObjectFast(const std::string& Name);

	static UObject* DefaultObjImpl(const UClass* TargetClass);

	static TArray<UObject*> GetObjectsOfClass(UClass* TargetClass, std::string MustIncludeString = "");

	static TArray<UObject*> FindObjects(const std::string& SearchString);
};

inline FUObjectArray GUObjectArray;

FORCEINLINE FUObjectItem* GetUObjectItem(const UObjectBase* Object)
{
	return Object ? FUObjectArray::IndexToObject(Object->GetInternalIndex()) : nullptr;
}

template<typename T>
class TObjectIterator
{
private:
	int32 Index;
	int32 MaxIndex;
	T* Current;

public:
	TObjectIterator()
		: Index(-1), MaxIndex(FUObjectArray::Num()), Current(nullptr)
	{
		Advance();
	}

	FORCEINLINE void operator++() { Advance(); }
	FORCEINLINE explicit operator bool() const { return Current != nullptr; }
	FORCEINLINE T* operator*() const { return Current; }
	FORCEINLINE T* operator->() const { return Current; }

	FORCEINLINE int32 GetIndex() const { return Index; }

private:
	void Advance()
	{
		Current = nullptr;
		while (++Index < MaxIndex)
		{
			FUObjectItem* Item = FUObjectArray::IndexToObject(Index);
			if (!Item || !Item->Object)
				continue;

			if (Item->Object->IsA(T::StaticClass()))
			{
				Current = (T*)Item->Object;
				return;
			}
		}
	}
};

template<typename T>
class TObjectRange
{
public:
	struct FSentinel {};

	struct FRangeIterator
	{
		TObjectIterator<T> It;

		FORCEINLINE T* operator*() const { return *It; }
		FORCEINLINE void operator++() { ++It; }
		FORCEINLINE bool operator!=(FSentinel) const { return (bool)It; }
	};

	FORCEINLINE FRangeIterator begin() const { return FRangeIterator{}; }
	FORCEINLINE FSentinel end() const { return FSentinel{}; }
};