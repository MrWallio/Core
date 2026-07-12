#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/Core/Public/HAL/UnrealMemory.h"

enum class EQueueMode
{
	Mpsc,
	Spsc
};

template<typename ItemType, EQueueMode Mode = EQueueMode::Spsc>
class TQueue
{
private:
	struct TNode
	{
		TNode* NextNode;
		ItemType Item;
	};

public:
	alignas(16) TNode* Head;
	TNode* Tail;

public:
	TQueue()
	{
		Head = Tail = AllocNode();
	}

	TQueue(const TQueue&) = delete;
	TQueue& operator=(const TQueue&) = delete;

private:
	static TNode* AllocNode()
	{
		TNode* Node = (TNode*)FMemory::Malloc(sizeof(TNode));
		std::memset((void*)Node, 0, sizeof(TNode));
		return Node;
	}

public:
	bool Enqueue(const ItemType& Item)
	{
		TNode* NewNode = AllocNode();
		std::memcpy((void*)&NewNode->Item, (const void*)&Item, sizeof(ItemType));

		TNode* OldHead = Head;
		Head = NewNode;
		OldHead->NextNode = NewNode;

		return true;
	}

	bool Dequeue(ItemType& OutItem)
	{
		TNode* Popped = Tail->NextNode;
		if (!Popped)
			return false;

		std::memcpy((void*)&OutItem, (const void*)&Popped->Item, sizeof(ItemType));

		TNode* OldTail = Tail;
		Tail = Popped;
		FMemory::Free(OldTail);

		return true;
	}

	bool Peek(ItemType& OutItem) const
	{
		TNode* Next = Tail->NextNode;
		if (!Next)
			return false;

		std::memcpy((void*)&OutItem, (const void*)&Next->Item, sizeof(ItemType));
		return true;
	}

	bool Pop()
	{
		TNode* Popped = Tail->NextNode;
		if (!Popped)
			return false;

		TNode* OldTail = Tail;
		Tail = Popped;
		FMemory::Free(OldTail);

		return true;
	}

	inline bool IsEmpty() const
	{
		return Tail->NextNode == nullptr;
	}

	void Empty()
	{
		while (Pop())
		{
		}
	}
};

static_assert(sizeof(TQueue<int32>) == 0x10, "TQueue layout broke: Head + Tail pointers expected (0x10 on x64)");
static_assert(alignof(TQueue<int32>) == 16, "TQueue alignment broke: UE aligns the Head pointer to 16");
