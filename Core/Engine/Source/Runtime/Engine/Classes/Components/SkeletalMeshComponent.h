#pragma once
#include "pch.h"

#include "SkinnedMeshComponent.h"
#include "Engine/Source/Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "Engine/Source/Runtime/Engine/Classes/Animation/AnimMontage.h"

class UAnimInstance;
class UAnimMontage;

class USkeletalMeshComponent : public USkinnedMeshComponent {
public:
	DefineUnrealClass(USkeletalMeshComponent);

	DefineUProperty(UAnimInstance*, AnimScriptInstance);
	DefineBitfieldUProperty(bNoSkeletonUpdate);
public:
	FORCEINLINE UAnimInstance* GetAnimInstance() const { return AnimScriptInstance; }
};
