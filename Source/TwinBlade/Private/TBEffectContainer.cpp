// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TBEffectContainer.h"
#include "AbilitySystemGlobals.h"
#include "TBAbilitySystemComponent.h"

void FTBEffectSpecContainer::SetTargetingResults(const TArray<FHitResult>& HitResults, const TArray<AActor*>& Actors) {
	// HitResults를 저장합니다.
	for (const FHitResult& HitResult : HitResults) {
		FGameplayAbilityTargetData_SingleTargetHit* NewData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
		TargetingResults.Add(NewData);
	}
	// Actors를 저장합니다.
	if (Actors.Num() > 0) {
		FGameplayAbilityTargetData_ActorArray* NewData = new FGameplayAbilityTargetData_ActorArray();
		NewData->TargetActorArray.Append(Actors);
		TargetingResults.Add(NewData);
	}
}

bool FTBEffectSpecContainer::HasValidTargetingResults() const {
	return TargetingResults.Num() > 0;
}

bool FTBEffectSpecContainer::HasValidEffectSpecs() const {
	return EffectSpecs.Num() > 0;
}