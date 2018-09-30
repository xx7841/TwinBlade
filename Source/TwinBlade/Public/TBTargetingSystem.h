// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "TwinBlade.h"
#include "GameplayAbilityTypes.h"
#include "TBTargetingSystem.generated.h"

class ATwinBladeCharacter;

UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class TWINBLADE_API UTBTargetingSystem : public UObject {
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	/* Target을 검출하는 알고리즘을 정의하세요. 블루프린트에서 정의가 가능합니다. */
	void GetTargets(ATwinBladeCharacter* CauserCharacter, AActor* CauserActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};

UCLASS(NotBlueprintable)
class TWINBLADE_API UTBTargetingSystem_Self : public UTBTargetingSystem {
	GENERATED_BODY()

public:
	/* Target이 자기 자신이라면 이 클래스의 이 함수를 호출하세요. */
	virtual void GetTargets_Implementation(ATwinBladeCharacter* CauserCharacter, AActor* CauserActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActor) const override;
};

UCLASS(NotBlueprintable)
class TWINBLADE_API UTBTargetingSystem_EventData : public UTBTargetingSystem {
	GENERATED_BODY()

public:
	/* EventData를 전달하여 Apply하는 Ability에는 이 클래스의 이 함수를 호출하세요. */
	virtual void GetTargets_Implementation(ATwinBladeCharacter* CauserCharacter, AActor* CauserActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResult, TArray<AActor*>& OutActor) const override;
};