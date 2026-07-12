#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/Core/Public/HAL/UnrealMemory.h"
#include "Engine/Source/Runtime/Core/Public/Templates/TypeCompatibleBytes.h"

inline int32 DefaultCalculateSlackGrow(int32 NumElements, int32 NumAllocatedElements)
{
    int32 Grow = 4;
    if (NumAllocatedElements || NumElements > Grow)
    {
        Grow = NumElements + 3 * NumElements / 8 + 16;
    }
    return Grow;
}

inline uint32 RoundUpToPowerOfTwo(uint32 Value)
{
    if (Value == 0)
        return 1;

    Value--;
    Value |= Value >> 1;
    Value |= Value >> 2;
    Value |= Value >> 4;
    Value |= Value >> 8;
    Value |= Value >> 16;
    return Value + 1;
}

class THeapAllocator
{
public:
    template<typename ElementType>
    class ForElementType
    {
    private:
        ElementType* Data;

    public:
        ForElementType()
            : Data(nullptr)
        {
        }

        ForElementType(ForElementType&&) = default;
        ForElementType(const ForElementType&) = default;

        ForElementType& operator=(ForElementType&&) = default;
        ForElementType& operator=(const ForElementType&) = default;

    public:
        inline const ElementType* GetAllocation() const { return Data; }
        inline ElementType* GetAllocation() { return Data; }

        void ResizeAllocation(int32 PreviousNumElements, int32 NumElements, size_t NumBytesPerElement)
        {
            if (Data || NumElements)
            {
                if (NumElements == 0)
                {
                    FMemory::Free(Data);
                    Data = nullptr;
                }
                else
                {
                    Data = (ElementType*)FMemory::Realloc(Data, (size_t)NumElements * NumBytesPerElement);
                }
            }
        }
    };
};

using FDefaultAllocator = THeapAllocator;

template<uint32 NumInlineElements>
class TInlineAllocator
{
public:
    template<typename ElementType>
    class ForElementType
    {
    private:
        static constexpr int32 ElementSize = sizeof(ElementType);
        static constexpr int32 ElementAlign = alignof(ElementType);

        static constexpr int32 InlineDataSizeBytes = NumInlineElements * ElementSize;

    private:
        TAlignedBytes<ElementSize, ElementAlign> InlineData[NumInlineElements];
        ElementType* SecondaryData;

    public:
        ForElementType()
            : InlineData{ 0x0 }, SecondaryData(nullptr)
        {
        }

        ForElementType(ForElementType&&) = default;
        ForElementType(const ForElementType&) = default;

    public:
        ForElementType& operator=(ForElementType&&) = default;
        ForElementType& operator=(const ForElementType&) = default;

    public:
        inline const ElementType* GetAllocation() const { return SecondaryData ? SecondaryData : reinterpret_cast<const ElementType*>(&InlineData); }
        inline ElementType* GetAllocation() { return SecondaryData ? SecondaryData : reinterpret_cast<ElementType*>(&InlineData); }

        inline uint32 GetNumInlineElements() const { return NumInlineElements; }

        void ResizeAllocation(int32 PreviousNumElements, int32 NumElements, size_t NumBytesPerElement)
        {
            if (NumElements <= (int32)NumInlineElements)
            {
                if (SecondaryData)
                {
                    const int32 NumToCopy = PreviousNumElements < (int32)NumInlineElements ? PreviousNumElements : (int32)NumInlineElements;
                    if (NumToCopy > 0)
                    {
                        std::memcpy((void*)InlineData, (const void*)SecondaryData, (size_t)NumToCopy * NumBytesPerElement);
                    }

                    FMemory::Free(SecondaryData);
                    SecondaryData = nullptr;
                }
            }
            else
            {
                if (!SecondaryData)
                {
                    SecondaryData = (ElementType*)FMemory::Malloc((size_t)NumElements * NumBytesPerElement);
                    if (PreviousNumElements > 0)
                    {
                        const int32 NumToCopy = PreviousNumElements < (int32)NumInlineElements ? PreviousNumElements : (int32)NumInlineElements;
                        std::memcpy((void*)SecondaryData, (const void*)InlineData, (size_t)NumToCopy * NumBytesPerElement);
                    }
                }
                else
                {
                    SecondaryData = (ElementType*)FMemory::Realloc(SecondaryData, (size_t)NumElements * NumBytesPerElement);
                }
            }
        }
    };
};

static_assert(sizeof(THeapAllocator::ForElementType<int32>) == 0x8, "THeapAllocator layout broke: single data pointer expected");
static_assert(sizeof(TInlineAllocator<1>::ForElementType<int32>) == 0x10, "TInlineAllocator<1> layout broke: 4 inline bytes + pad + secondary pointer = 0x10");
static_assert(sizeof(TInlineAllocator<4>::ForElementType<int32>) == 0x18, "TInlineAllocator<4> layout broke: 16 inline bytes + secondary pointer = 0x18");
