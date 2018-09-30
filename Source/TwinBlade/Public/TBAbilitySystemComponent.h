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
	/* AbilitySpecs �� Tags�� ��ġ�ϴ� ��� Abilities�� ��ȯ�մϴ�. */
	void GetActiveAbilitiesByTags(const FGameplayTagContainer& Tags, TArray<UTBAbility*>& OutActiveAbilities);

	/* Actor�� ������ AbilitySystemComponent�� ��ȯ�մϴ�. */
	static UTBAbilitySystemComponent* GetAbilitySystemComponentByActor(const AActor* Actor);
};