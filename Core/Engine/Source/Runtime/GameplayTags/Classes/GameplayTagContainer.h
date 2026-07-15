#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"

struct FGameplayTagContainer;

enum class EGameplayContainerMatchType : uint8
{
	Any,
	All
};

struct FGameplayTag {
public:
	DefineUnrealStruct(FGameplayTag);
public:
	FGameplayTag()
	{
	}

	explicit FGameplayTag(FName InTagName)
		: TagName(InTagName)
	{
	}

	inline bool operator==(FGameplayTag const& Other) const
	{
		return TagName == Other.TagName;
	}

	inline bool operator!=(FGameplayTag const& Other) const
	{
		return TagName != Other.TagName;
	}

	inline bool MatchesTagExact(const FGameplayTag& TagToCheck) const
	{
		if (!TagToCheck.IsValid())
		{
			return false;
		}
		return TagName == TagToCheck.TagName;
	}

	bool MatchesTag(const FGameplayTag& TagToCheck) const
	{
		if (!TagToCheck.IsValid() || !IsValid())
		{
			return false;
		}

		if (TagName == TagToCheck.TagName)
		{
			return true;
		}

		FString ThisName = TagName.ToString();
		FString ParentName = TagToCheck.TagName.ToString();

		return ThisName.Len() > ParentName.Len()
			&& ThisName[ParentName.Len()] == L'.'
			&& ThisName.StartsWith(ParentName, ESearchCase::IgnoreCase);
	}

	bool MatchesAny(const FGameplayTagContainer& ContainerToCheck) const;

	bool MatchesAnyExact(const FGameplayTagContainer& ContainerToCheck) const;

	inline bool IsValid() const
	{
		return TagName != FName();
	}

	inline FString ToString() const
	{
		return TagName.ToString();
	}

	inline FName GetTagName() const
	{
		return TagName;
	}
public:
	FName TagName;
};

struct FGameplayTagContainer {
public:
	DefineUnrealStruct(FGameplayTagContainer);
public:
	FGameplayTagContainer()
	{
	}

	FGameplayTagContainer(FGameplayTagContainer const& Other)
	{
		*this = Other;
	}

	explicit FGameplayTagContainer(const FGameplayTag& Tag)
	{
		AddTag(Tag);
	}

	~FGameplayTagContainer()
	{
	}

	inline int32 Num() const
	{
		return GameplayTags.Num();
	}

	inline bool IsValid() const
	{
		return GameplayTags.Num() > 0;
	}

	inline bool IsEmpty() const
	{
		return GameplayTags.Num() == 0;
	}

	bool IsValidIndex(int32 Index) const
	{
		return GameplayTags.IsValidIndex(Index);
	}

	FGameplayTag GetByIndex(int32 Index) const
	{
		if (IsValidIndex(Index))
		{
			return GameplayTags[Index];
		}
		return FGameplayTag();
	}

	FGameplayTag First() const
	{
		return GameplayTags.Num() > 0 ? GameplayTags[0] : FGameplayTag();
	}

	FGameplayTag Last() const
	{
		return GameplayTags.Num() > 0 ? GameplayTags[GameplayTags.Num() - 1] : FGameplayTag();
	}

	void Reset() {
		GameplayTags.Reset();
		ParentTags.Reset();
	}

	bool HasTag(const FGameplayTag& TagToCheck) const
	{
		if (!TagToCheck.IsValid())
		{
			return false;
		}

		return GameplayTags.Contains(TagToCheck) || ParentTags.Contains(TagToCheck);
	}

	bool HasTagExact(const FGameplayTag& TagToCheck) const
	{
		if (!TagToCheck.IsValid())
		{
			return false;
		}

		return GameplayTags.Contains(TagToCheck);
	}

	bool HasAny(const FGameplayTagContainer& ContainerToCheck) const
	{
		if (ContainerToCheck.IsEmpty())
		{
			return false;
		}
		for (const FGameplayTag& OtherTag : ContainerToCheck.GameplayTags)
		{
			if (GameplayTags.Contains(OtherTag) || ParentTags.Contains(OtherTag))
			{
				return true;
			}
		}
		return false;
	}

	bool HasAnyExact(const FGameplayTagContainer& ContainerToCheck) const
	{
		if (ContainerToCheck.IsEmpty())
		{
			return false;
		}
		for (const FGameplayTag& OtherTag : ContainerToCheck.GameplayTags)
		{
			if (GameplayTags.Contains(OtherTag))
			{
				return true;
			}
		}
		return false;
	}

	bool HasAll(const FGameplayTagContainer& ContainerToCheck) const
	{
		if (ContainerToCheck.IsEmpty())
		{
			return true;
		}
		for (const FGameplayTag& OtherTag : ContainerToCheck.GameplayTags)
		{
			if (!GameplayTags.Contains(OtherTag) && !ParentTags.Contains(OtherTag))
			{
				return false;
			}
		}
		return true;
	}

	bool HasAllExact(const FGameplayTagContainer& ContainerToCheck) const
	{
		if (ContainerToCheck.IsEmpty())
		{
			return true;
		}
		for (const FGameplayTag& OtherTag : ContainerToCheck.GameplayTags)
		{
			if (!GameplayTags.Contains(OtherTag))
			{
				return false;
			}
		}
		return true;
	}

	void AddTag(const FGameplayTag& TagToAdd)
	{
		if (TagToAdd.IsValid())
		{
			GameplayTags.AddUnique(TagToAdd);
			AddParentsForTag(TagToAdd);
		}
	}

	void AddTagFast(const FGameplayTag& TagToAdd)
	{
		GameplayTags.Add(TagToAdd);
		AddParentsForTag(TagToAdd);
	}

	bool RemoveTag(const FGameplayTag& TagToRemove)
	{
		for (int32 Index = 0; Index < GameplayTags.Num(); ++Index)
		{
			if (GameplayTags[Index] == TagToRemove)
			{
				GameplayTags.RemoveAt(Index);
				FillParentTags();
				return true;
			}
		}

		return false;
	}

	void AppendTags(const FGameplayTagContainer& Other)
	{
		for (const FGameplayTag& OtherTag : Other.GameplayTags)
		{
			GameplayTags.AddUnique(OtherTag);
		}
		for (const FGameplayTag& OtherParentTag : Other.ParentTags)
		{
			ParentTags.AddUnique(OtherParentTag);
		}
	}
protected:
	void AddParentsForTag(const FGameplayTag& Tag)
	{
		std::string TagString = Tag.TagName.ToString().ToString();

		size_t DotPos = TagString.rfind('.');
		while (DotPos != std::string::npos)
		{
			TagString = TagString.substr(0, DotPos);

			std::wstring WideTagString(TagString.begin(), TagString.end());
			FGameplayTag ParentTag(UKismetStringLibrary::Conv_StringToName(WideTagString.c_str()));
			ParentTags.AddUnique(ParentTag);

			DotPos = TagString.rfind('.');
		}
	}

	void FillParentTags()
	{
		ParentTags.Reset();
		for (const FGameplayTag& Tag : GameplayTags)
		{
			AddParentsForTag(Tag);
		}
	}
public:
	TArray<FGameplayTag> GameplayTags;

	TArray<FGameplayTag> ParentTags;
};

inline bool FGameplayTag::MatchesAny(const FGameplayTagContainer& ContainerToCheck) const
{
	for (const FGameplayTag& OtherTag : ContainerToCheck.GameplayTags)
	{
		if (MatchesTag(OtherTag))
		{
			return true;
		}
	}

	return false;
}

inline bool FGameplayTag::MatchesAnyExact(const FGameplayTagContainer& ContainerToCheck) const
{
	if (!IsValid())
	{
		return false;
	}

	return ContainerToCheck.GameplayTags.Contains(*this);
}

static_assert(sizeof(FGameplayTag) == 0x8, "FGameplayTag layout broke: single FName expected (0x8)");
static_assert(sizeof(FGameplayTagContainer) == 0x20, "FGameplayTagContainer layout broke: GameplayTags + ParentTags arrays expected (0x20)");
