// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "TwinBlade.h"
#include "AbilitySystemComponent.h"
#include "TBAbilitySystemComponent.generated.h"

class UTBAbility;

UCLASS()
class TWINBLADE_API UTBAbilitySystemComponent : public UAbilitySystemComponent {
	GENERATED_BODY()

public:
	/* AbilitySpecs 중 Tags과 일치하는 모든 Abilities를 반환합니다. */
	void GetActiveAbilitiesByTags(const FGameplayTagContainer& Tags, TArray<UTBAbility*>& OutActiveAbilities);

	/* Actor가 소유한 AbilitySystemComponent를 반환합니다. */
	static UTBAbilitySystemComponent* GetAbilitySystemComponentByActor(const AActor* Actor);
};