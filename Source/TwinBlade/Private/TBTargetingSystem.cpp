// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TBTargetingSystem.h"
#include "TwinBladeCharacter.h"

void UTBTargetingSystem::GetTargets_Implementation(ATwinBladeCharacter* CauserCharacter, AActor* CauserActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const {
	return;
}

void UTBTargetingSystem_Self::GetTargets_Implementation(ATwinBladeCharacter* CauserCharacter, AActor* CauserActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActor) const {
	// Target은 자기자신입니다.
	OutActor.Add(CauserCharacter);
}

void UTBTargetingSystem_EventData::GetTargets_Implementation(ATwinBladeCharacter* CauserCharacter, AActor* CauserActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResult, TArray<AActor*>& OutActor) const {
	// EventData를 해석하여 HitResult나 Actor를 얻습니다.
	const FHitResult* FoundHitResult = EventData.ContextHandle.GetHitResult();
	if (FoundHitResult) {
		OutHitResult.Add(*FoundHitResult);
	}
	else if (EventData.Target) {
		OutActor.Add(const_cast<AActor*>(EventData.Target));
	}
}