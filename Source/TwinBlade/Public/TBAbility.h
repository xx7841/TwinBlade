// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "TwinBlade.h"
#include "GameplayAbility.h"
#include "TBEffectContainer.h"
#include "GameplayTagContainer.h"
#include "TBAbility.generated.h"

UCLASS()
class TWINBLADE_API UTBAbility : public UGameplayAbility {
	GENERATED_BODY()
	
public:
	/* �ϳ��� Ability�� ���� ���� FTBEffectContainer���� ������ �� �ֽ��ϴ�. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FTBEffectContainer> EffectContainerMap;

	/* Ability�� ����� �� Ability�� ���� Effects�� TargetingResults���� ����� ���� ����� ��ȯ�մϴ�. */
	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "EventData"))
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainer(FGameplayTag Tag, const FGameplayEventData& EventData);

	/* Ability�� ����� �� Ȱ��ȭ�� FTBEffectContainer�� �̿��� EffectSpecs�� TargetingResults�� ���մϴ�.*/
	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "EventData"))
	virtual FTBEffectSpecContainer GetEffectSpecContainer(FGameplayTag Tag, const FGameplayEventData& EventData);
};