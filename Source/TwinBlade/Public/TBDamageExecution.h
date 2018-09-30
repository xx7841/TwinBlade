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

	/* �������� Effect�� ����� �� �ڵ����� ȣ��Ǵ� �Լ��Դϴ�. */
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};