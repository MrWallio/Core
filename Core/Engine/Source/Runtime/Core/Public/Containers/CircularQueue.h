#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/Core/Public/Containers/CircularBuffer.h"

// Layout mirrors UE's TCircularQueue: a circular buffer plus Head/Tail cursors.
// This mirror performs the cursor math without the engine's atomics -- treat
// cross-thread use as unsafe.
template<typename ItemType>
class TCircularQueue
{
public:
	TCircularBuffer<ItemType> Buffer;
	uint32 Head;
	uint32 Tail;

public:
	TCircularQueue()
		: Head(0), Tail(0)
	{
	}

	explicit TCircularQueue(uint32 CapacityPlusOne)
		: Buffer(CapacityPlusOne), Head(0), Tail(0)
	{
	}

public:
	inline uint32 Count() const
	{
		return (Tail > Head ? Buffer.Capacity() : 0) + Head - Tail;
	}

	inline bool IsEmpty() const { return Head == Tail; }

	inline bool IsFull() const { return Buffer.GetNextIndex(Head) == Tail; }

public:
	bool Enqueue(const ItemType& Item)
	{
		const uint32 NewHead = Buffer.GetNextIndex(Head);
		if (NewHead == Tail)
			return false;

		Buffer[Head] = Item;
		Head = NewHead;
		return true;
	}

	bool Dequeue(ItemType& OutItem)
	{
		if (Head == Tail)
			return false;

		OutItem = Buffer[Tail];
		Tail = Buffer.GetNextIndex(Tail);
		return true;
	}

	bool Peek(ItemType& OutItem) const
	{
		if (Head == Tail)
			return false;

		OutItem = Buffer[Tail];
		return true;
	}

	inline void Empty()
	{
		Head = Tail = 0;
	}
};

static_assert(sizeof(TCircularQueue<int32>) == 0x20, "TCircularQueue layout broke: buffer + Head + Tail expected (0x20 on x64)");
