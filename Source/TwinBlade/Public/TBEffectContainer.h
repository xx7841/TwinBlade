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
	/* Targeting �� ����� ��������Դϴ�. Actor�� HitResult�� ���� �� �ֽ��ϴ�. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAbilityTargetDataHandle TargetingResults;

	/* TargetingResults���� ������ EffectSpecs �Դϴ�. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FGameplayEffectSpecHandle> EffectSpecs;

	/* TargetingResults�� �����մϴ�. */
	void SetTargetingResults(const TArray<FHitResult>& HitResults, const TArray<AActor*>& Actors);

	/* TargetingResults�� ��ȿ�մϱ�? */
	bool HasValidTargetingResults() const;

	/* EffectSpecs�� ��ȿ�մϱ�? */
	bool HasValidEffectSpecs() const;
};