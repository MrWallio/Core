#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Core/Public/Templates/TypeCompatibleBytes.h"
#include "SparseArray.h"
#include "BitArray.h"
#include "ContainerIterator.h"

template <typename SetType>
class SetElement
{
private:
    template <typename SetDataType>
    friend class TSet;

public:
    SetType Value;
    int32_t HashNextId;
    int32_t HashIndex;

public:
    SetElement() : Value(), HashNextId(-1), HashIndex(-1) {}

    SetElement(const SetType& InValue)
        : Value(InValue)
        , HashNextId(-1)
        , HashIndex(-1)
    {
    }
};

template <typename SetElementType>
class TSet
{
private:
    static constexpr uint32_t ElementAlign = alignof(SetElementType);
    static constexpr uint32_t ElementSize = sizeof(SetElementType);

private:
    using SetDataType = SetElement<SetElementType>;
    using HashType = TInlineAllocator<1>::ForElementType<int32_t>;

public:
    TSparseArray<SetDataType> Elements;
    HashType Hash;
    int32_t HashSize;

public:
    TSet()
        : HashSize(0)
    {
    }

private:
    inline void VerifyIndex(int32 Index) const { if (!IsValidIndex(Index)) throw std::out_of_range("Index was out of range!"); }

public:
    inline int32 NumAllocated() const { return Elements.NumAllocated(); }

    inline int32 Num() const { return Elements.Num(); }
    inline int32 Max() const { return Elements.Max(); }

    inline bool IsValidIndex(int32 Index) const { return Elements.IsValidIndex(Index); }

    inline bool IsValid() const { return Elements.IsValid(); }

    const FBitArray& GetAllocationFlags() const { return Elements.GetAllocationFlags(); }

public:
    inline       SetElementType& operator[] (int32 Index) { return Elements[Index].Value; }
    inline const SetElementType& operator[] (int32 Index) const { return Elements[Index].Value; }

    inline bool operator==(const TSet<SetElementType>& Other) const { return Elements == Other.Elements; }
    inline bool operator!=(const TSet<SetElementType>& Other) const { return Elements != Other.Elements; }

public:
    const SetElementType* Find(const SetElementType& Item) const
    {
        const FBitArray& AllocationFlags = Elements.GetAllocationFlags();

        for (ContainerIterators::FSetBitIterator It(AllocationFlags); It; ++It)
        {
            int32 Index = It.GetIndex();
            if (Elements[Index].Value == Item)
            {
                return &Elements[Index].Value;
            }
        }

        return nullptr;
    }

    SetElementType* Find(const SetElementType& Item)
    {
        const FBitArray& AllocationFlags = Elements.GetAllocationFlags();

        for (ContainerIterators::FSetBitIterator It(AllocationFlags); It; ++It)
        {
            int32 Index = It.GetIndex();
            if (Elements[Index].Value == Item)
            {
                return &Elements[Index].Value;
            }
        }

        return nullptr;
    }

    template <typename ComparisonType>
    bool Contains(ComparisonType Item) const
    {
        const FBitArray& AllocationFlags = Elements.GetAllocationFlags();

        for (ContainerIterators::FSetBitIterator It(AllocationFlags); It; ++It)
        {
            int32 Index = It.GetIndex();
            if (Elements[Index].Value == Item)
            {
                return true;
            }
        }

        return false;
    }

    bool Remove(const SetElementType& Item)
    {
        const FBitArray& AllocationFlags = Elements.GetAllocationFlags();

        for (ContainerIterators::FSetBitIterator It(AllocationFlags); It; ++It)
        {
            int32 Index = It.GetIndex();
            if (Elements[Index].Value == Item)
            {
                Elements.RemoveAt(Index);
                return true;
            }
        }
        return false;
    }

    void RemoveAt(int32 Index)
    {
        if (Elements.IsValidIndex(Index))
        {
            Elements.RemoveAt(Index);
        }
    }
public:
    template<typename T> friend ContainerIterators::TSetIterator<T> begin(const TSet& Set);
    template<typename T> friend ContainerIterators::TSetIterator<T> end(const TSet& Set);
};

template<typename T> inline ContainerIterators::TSetIterator<T> begin(const TSet<T>& Set) { return ContainerIterators::TSetIterator<T>(Set, Set.GetAllocationFlags(), 0); }
template<typename T> inline ContainerIterators::TSetIterator<T> end(const TSet<T>& Set) { return ContainerIterators::TSetIterator<T>(Set, Set.GetAllocationFlags(), Set.NumAllocated()); }