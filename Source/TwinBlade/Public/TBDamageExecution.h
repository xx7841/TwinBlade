// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "TwinBlade.h"
#include "GameplayEffectExecutionCalculation.h"
#include "TBDamageExecution.generated.h"

UCLASS()
class TWINBLADE_API UTBDamageExecution : public UGameplayEffectExecutionCalculation {
	GENERATED_BODY()

public:
	UTBDamageExecution();

	/* 소유중인 Effect가 실행될 때 자동으로 호출되는 함수입니다. */
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};