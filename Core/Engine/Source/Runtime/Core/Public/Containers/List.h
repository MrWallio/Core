#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/Core/Public/HAL/UnrealMemory.h"

template<typename ElementType>
class TLinkedList
{
public:
	TLinkedList* NextLink;
	TLinkedList** PrevLink;
	ElementType Element;

public:
	TLinkedList()
		: NextLink(nullptr), PrevLink(nullptr), Element()
	{
	}

	explicit TLinkedList(const ElementType& InElement)
		: NextLink(nullptr), PrevLink(nullptr), Element(InElement)
	{
	}

public:
	inline TLinkedList* Next() { return NextLink; }

	inline bool IsLinked() const { return PrevLink != nullptr; }

	void LinkHead(TLinkedList*& Head)
	{
		if (Head)
		{
			Head->PrevLink = &NextLink;
		}

		NextLink = Head;
		PrevLink = &Head;
		Head = this;
	}

	void Unlink()
	{
		if (NextLink)
		{
			NextLink->PrevLink = PrevLink;
		}
		if (PrevLink)
		{
			*PrevLink = NextLink;
		}

		NextLink = nullptr;
		PrevLink = nullptr;
	}

public:
	inline ElementType& operator*() { return Element; }
	inline ElementType* operator->() { return &Element; }
	inline const ElementType& operator*() const { return Element; }
	inline const ElementType* operator->() const { return &Element; }

public:
	class TIterator
	{
	private:
		TLinkedList* CurrentLink;

	public:
		explicit TIterator(TLinkedList* FirstLink) : CurrentLink(FirstLink) {}

		inline TIterator& operator++() { CurrentLink = CurrentLink->NextLink; return *this; }

		inline explicit operator bool() const { return CurrentLink != nullptr; }

		inline ElementType& operator*() const { return CurrentLink->Element; }
		inline ElementType* operator->() const { return &CurrentLink->Element; }

		inline bool operator==(const TIterator& Other) const { return CurrentLink == Other.CurrentLink; }
		inline bool operator!=(const TIterator& Other) const { return CurrentLink != Other.CurrentLink; }
	};
};

template<typename ElementType>
class TDoubleLinkedList
{
public:
	class TDoubleLinkedListNode
	{
	public:
		ElementType Value;
		TDoubleLinkedListNode* NextNode;
		TDoubleLinkedListNode* PrevNode;

	public:
		TDoubleLinkedListNode(const ElementType& InValue)
			: Value(InValue), NextNode(nullptr), PrevNode(nullptr)
		{
		}

		inline ElementType& GetValue() { return Value; }
		inline const ElementType& GetValue() const { return Value; }

		inline TDoubleLinkedListNode* GetNextNode() { return NextNode; }
		inline TDoubleLinkedListNode* GetPrevNode() { return PrevNode; }
	};

public:
	TDoubleLinkedListNode* HeadNode;
	TDoubleLinkedListNode* TailNode;
	int32 ListSize;

public:
	TDoubleLinkedList()
		: HeadNode(nullptr), TailNode(nullptr), ListSize(0)
	{
	}

	TDoubleLinkedList(const TDoubleLinkedList&) = delete;
	TDoubleLinkedList& operator=(const TDoubleLinkedList&) = delete;

private:
	static TDoubleLinkedListNode* AllocNode(const ElementType& InValue)
	{
		TDoubleLinkedListNode* Node = (TDoubleLinkedListNode*)FMemory::Malloc(sizeof(TDoubleLinkedListNode));
		std::memset((void*)Node, 0, sizeof(TDoubleLinkedListNode));
		std::memcpy((void*)&Node->Value, (const void*)&InValue, sizeof(ElementType));
		return Node;
	}

public:
	inline int32 Num() const { return ListSize; }

	inline bool IsEmpty() const { return ListSize == 0; }

	inline TDoubleLinkedListNode* GetHead() const { return HeadNode; }
	inline TDoubleLinkedListNode* GetTail() const { return TailNode; }

public:
	bool AddHead(const ElementType& InElement)
	{
		TDoubleLinkedListNode* NewNode = AllocNode(InElement);

		if (HeadNode)
		{
			NewNode->NextNode = HeadNode;
			HeadNode->PrevNode = NewNode;
			HeadNode = NewNode;
		}
		else
		{
			HeadNode = TailNode = NewNode;
		}

		++ListSize;
		return true;
	}

	bool AddTail(const ElementType& InElement)
	{
		TDoubleLinkedListNode* NewNode = AllocNode(InElement);

		if (TailNode)
		{
			NewNode->PrevNode = TailNode;
			TailNode->NextNode = NewNode;
			TailNode = NewNode;
		}
		else
		{
			HeadNode = TailNode = NewNode;
		}

		++ListSize;
		return true;
	}

	void RemoveNode(TDoubleLinkedListNode* NodeToRemove)
	{
		if (!NodeToRemove)
			return;

		if (NodeToRemove == HeadNode)
		{
			HeadNode = NodeToRemove->NextNode;
		}
		if (NodeToRemove == TailNode)
		{
			TailNode = NodeToRemove->PrevNode;
		}

		if (NodeToRemove->NextNode)
		{
			NodeToRemove->NextNode->PrevNode = NodeToRemove->PrevNode;
		}
		if (NodeToRemove->PrevNode)
		{
			NodeToRemove->PrevNode->NextNode = NodeToRemove->NextNode;
		}

		FMemory::Free(NodeToRemove);
		--ListSize;
	}

	bool RemoveNode(const ElementType& InElement)
	{
		TDoubleLinkedListNode* Node = FindNode(InElement);
		if (!Node)
			return false;

		RemoveNode(Node);
		return true;
	}

	TDoubleLinkedListNode* FindNode(const ElementType& InElement) const
	{
		for (TDoubleLinkedListNode* Node = HeadNode; Node; Node = Node->NextNode)
		{
			if (Node->Value == InElement)
			{
				return Node;
			}
		}

		return nullptr;
	}

	bool Contains(const ElementType& InElement) const
	{
		return FindNode(InElement) != nullptr;
	}

	void Empty()
	{
		TDoubleLinkedListNode* Node = HeadNode;
		while (Node)
		{
			TDoubleLinkedListNode* Next = Node->NextNode;
			FMemory::Free(Node);
			Node = Next;
		}

		HeadNode = nullptr;
		TailNode = nullptr;
		ListSize = 0;
	}

public:
	class TIterator
	{
	private:
		TDoubleLinkedListNode* CurrentNode;

	public:
		explicit TIterator(TDoubleLinkedListNode* StartingNode) : CurrentNode(StartingNode) {}

		inline TIterator& operator++() { CurrentNode = CurrentNode->NextNode; return *this; }

		inline explicit operator bool() const { return CurrentNode != nullptr; }

		inline ElementType& operator*() const { return CurrentNode->Value; }
		inline ElementType* operator->() const { return &CurrentNode->Value; }

		inline bool operator==(const TIterator& Other) const { return CurrentNode == Other.CurrentNode; }
		inline bool operator!=(const TIterator& Other) const { return CurrentNode != Other.CurrentNode; }
	};

	inline TIterator begin() { return TIterator(HeadNode); }
	inline TIterator end() { return TIterator(nullptr); }
	inline TIterator begin() const { return TIterator(HeadNode); }
	inline TIterator end() const { return TIterator(nullptr); }
};

static_assert(sizeof(TDoubleLinkedList<int32>) == 0x18, "TDoubleLinkedList layout broke: HeadNode/TailNode/ListSize expected (0x18 on x64)");
