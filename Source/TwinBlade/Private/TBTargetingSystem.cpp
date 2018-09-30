// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TBTargetingSystem.h"
#include "TwinBladeCharacter.h"

void UTBTargetingSystem::GetTargets_Implementation(ATwinBladeCharacter* CauserCharacter, AActor* CauserActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const {
	return;
}

void UTBTargetingSystem_Self::GetTargets_Implementation(ATwinBladeCharacter* CauserCharacter, AActor* CauserActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActor) const {
	// Target�� �ڱ��ڽ��Դϴ�.
	OutActor.Add(CauserCharacter);
}

void UTBTargetingSystem_EventData::GetTargets_Implementation(ATwinBladeCharacter* CauserCharacter, AActor* CauserActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResult, TArray<AActor*>& OutActor) const {
	// EventData�� �ؼ��Ͽ� HitResult�� Actor�� ����ϴ�.
	const FHitResult* FoundHitResult = EventData.ContextHandle.GetHitResult();
	if (FoundHitResult) {
		OutHitResult.Add(*FoundHitResult);
	}
	else if (EventData.Target) {
		OutActor.Add(const_cast<AActor*>(EventData.Target));
	}
}