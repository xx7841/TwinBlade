// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TBAbilitySystemComponent.h"
#include "TBAbility.h"
#include "AbilitySystemGlobals.h"

void UTBAbilitySystemComponent::GetActiveAbilitiesByTags(const FGameplayTagContainer& Tags, TArray<UTBAbility*>& OutActiveAbilities) {
	TArray<FGameplayAbilitySpec*> AbilitySpecs;
	// Tags과 일치하는 모든 AbilitySpecs를 불러옵니다.
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(Tags, AbilitySpecs, false);

	// 불러온 모든 AbilitySpecs에 대해...
	for (FGameplayAbilitySpec* AbilitySpec : AbilitySpecs) {
		TArray<UGameplayAbility*> AbilityInstances = AbilitySpec->GetAbilityInstances();
		// Ability 객체를 저장합니다.
		for (UGameplayAbility* ActiveAbility : AbilityInstances) {
			OutActiveAbilities.Add(Cast<UTBAbility>(ActiveAbility));
		}
	}
}

UTBAbilitySystemComponent* UTBAbilitySystemComponent::GetAbilitySystemComponentByActor(const AActor* Actor) {
	return Cast<UTBAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor));
}