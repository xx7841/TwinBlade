// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TBDamageExecution.h"
#include "TBAttributeSet.h"
#include "TBAbilitySystemComponent.h"

struct TBDamageStatics {
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower);

	TBDamageStatics() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTBAttributeSet, Damage, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTBAttributeSet, AttackPower, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTBAttributeSet, DefensePower, Target, false);
	}
};

static const TBDamageStatics& DamageStatics() {
	static TBDamageStatics DamageStatics;
	return DamageStatics;
}

UTBDamageExecution::UTBDamageExecution() {
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().AttackPowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().DefensePowerDef);
}

void UTBDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const {
	UAbilitySystemComponent* CauserAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* CauserActor = CauserAbilitySystemComponent ? CauserAbilitySystemComponent->AvatarActor : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->AvatarActor : nullptr;

	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* CauserTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = CauserTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Blueprint의 Modifier Magnitude에서 설정한 수치/방식으로 변경한다.
	float AttackPower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackPowerDef, EvaluationParameters, AttackPower);

	float DefensePower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefensePowerDef, EvaluationParameters, DefensePower);
	if (DefensePower == 0.0f) {
		DefensePower = 1.0f;
	}

	float DamageDone = AttackPower / DefensePower;
	if (DamageDone > 0.0f) {
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Override, DamageDone));
	}
}