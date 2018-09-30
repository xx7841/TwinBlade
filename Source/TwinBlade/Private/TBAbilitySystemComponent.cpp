// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TBAbilitySystemComponent.h"
#include "TBAbility.h"
#include "AbilitySystemGlobals.h"

void UTBAbilitySystemComponent::GetActiveAbilitiesByTags(const FGameplayTagContainer& Tags, TArray<UTBAbility*>& OutActiveAbilities) {
	TArray<FGameplayAbilitySpec*> AbilitySpecs;
	// Tags�� ��ġ�ϴ� ��� AbilitySpecs�� �ҷ��ɴϴ�.
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(Tags, AbilitySpecs, false);

	// �ҷ��� ��� AbilitySpecs�� ����...
	for (FGameplayAbilitySpec* AbilitySpec : AbilitySpecs) {
		TArray<UGameplayAbility*> AbilityInstances = AbilitySpec->GetAbilityInstances();
		// Ability ��ü�� �����մϴ�.
		for (UGameplayAbility* ActiveAbility : AbilityInstances) {
			OutActiveAbilities.Add(Cast<UTBAbility>(ActiveAbility));
		}
	}
}

UTBAbilitySystemComponent* UTBAbilitySystemComponent::GetAbilitySystemComponentByActor(const AActor* Actor) {
	return Cast<UTBAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor));
}