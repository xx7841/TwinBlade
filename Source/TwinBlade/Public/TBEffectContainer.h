// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "TwinBlade.h"
#include "GameplayEffectTypes.h"
#include "GameplayAbilityTargetTypes.h"
#include "TBEffectContainer.generated.h"

class UTBTargetingSystem;

USTRUCT(BlueprintType)
struct FTBEffectContainer {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UTBTargetingSystem> TargetingSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayEffect>> Effects;
};

USTRUCT(BlueprintType)
struct FTBEffectSpecContainer {
	GENERATED_BODY()

public:
	/* Targeting 후 검출된 결과물들입니다. Actor와 HitResult가 있을 수 있습니다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAbilityTargetDataHandle TargetingResults;

	/* TargetingResults에게 적용할 EffectSpecs 입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FGameplayEffectSpecHandle> EffectSpecs;

	/* TargetingResults를 저장합니다. */
	void SetTargetingResults(const TArray<FHitResult>& HitResults, const TArray<AActor*>& Actors);

	/* TargetingResults가 유효합니까? */
	bool HasValidTargetingResults() const;

	/* EffectSpecs가 유효합니까? */
	bool HasValidEffectSpecs() const;
};