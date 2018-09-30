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
	/* 하나의 Ability에 여러 개의 FTBEffectContainer들을 저장할 수 있습니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FTBEffectContainer> EffectContainerMap;

	/* Ability가 실행될 때 Ability가 가진 Effects가 TargetingResults에게 적용된 후의 결과를 반환합니다. */
	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "EventData"))
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainer(FGameplayTag Tag, const FGameplayEventData& EventData);

	/* Ability가 실행될 때 활성화할 FTBEffectContainer를 이용해 EffectSpecs와 TargetingResults를 구합니다.*/
	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "EventData"))
	virtual FTBEffectSpecContainer GetEffectSpecContainer(FGameplayTag Tag, const FGameplayEventData& EventData);
};