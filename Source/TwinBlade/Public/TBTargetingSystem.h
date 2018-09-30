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
	/* Target�� �����ϴ� �˰����� �����ϼ���. �������Ʈ���� ���ǰ� �����մϴ�. */
	void GetTargets(ATwinBladeCharacter* CauserCharacter, AActor* CauserActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};

UCLASS(NotBlueprintable)
class TWINBLADE_API UTBTargetingSystem_Self : public UTBTargetingSystem {
	GENERATED_BODY()

public:
	/* Target�� �ڱ� �ڽ��̶�� �� Ŭ������ �� �Լ��� ȣ���ϼ���. */
	virtual void GetTargets_Implementation(ATwinBladeCharacter* CauserCharacter, AActor* CauserActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActor) const override;
};

UCLASS(NotBlueprintable)
class TWINBLADE_API UTBTargetingSystem_EventData : public UTBTargetingSystem {
	GENERATED_BODY()

public:
	/* EventData�� �����Ͽ� Apply�ϴ� Ability���� �� Ŭ������ �� �Լ��� ȣ���ϼ���. */
	virtual void GetTargets_Implementation(ATwinBladeCharacter* CauserCharacter, AActor* CauserActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResult, TArray<AActor*>& OutActor) const override;
};