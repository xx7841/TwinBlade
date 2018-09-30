// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "TwinBlade.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TBAbilityTask_PlayMontageAndWaitForEvent.generated.h"

class UTBAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTBPlayMontageAndWaitForEventDelegate, FGameplayTag, EventTag, FGameplayEventData, EventData);

UCLASS()
class TWINBLADE_API UTBAbilityTask_PlayMontageAndWaitForEvent : public UAbilityTask {
	GENERATED_BODY()
	
public:
	UTBAbilityTask_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer);

	// Delegate
	UPROPERTY(BlueprintAssignable)
	FTBPlayMontageAndWaitForEventDelegate OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FTBPlayMontageAndWaitForEventDelegate OnBlendOut;

	UPROPERTY(BlueprintAssignable)
	FTBPlayMontageAndWaitForEventDelegate OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FTBPlayMontageAndWaitForEventDelegate OnCancelled;

	UPROPERTY(BlueprintAssignable)
	FTBPlayMontageAndWaitForEventDelegate EventReceived;

	virtual void Activate() override;

	virtual void ExternalCancel() override;

	virtual void OnDestroy(bool bIsAbilityEnd) override;

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UTBAbilityTask_PlayMontageAndWaitForEvent* PlayMontageAndWaitForEvent(UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay, FGameplayTagContainer EventTags, float Rate = 1.0f, FName StartSection = NAME_None, bool bStopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.0f);

private:
	void OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload);

	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	void OnAbilityCancelled();

	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	bool StopPlayingMontage();

	UTBAbilitySystemComponent* GetTargetAbilitySystemComponent();

	UPROPERTY()
	UAnimMontage* MontageToPlay;

	UPROPERTY()
	FGameplayTagContainer EventTags;

	UPROPERTY()
	float Rate;

	UPROPERTY()
	FName StartSection;

	UPROPERTY()
	float AnimRootMotionTranslationScale;

	UPROPERTY()
	bool bIsStopMontageWhenAbilityEnds;

	FDelegateHandle EventHandle;

	FDelegateHandle CancelledHandle;

	FOnMontageBlendingOutStarted BlendingOutDelegate;

	FOnMontageEnded MontageEndedDelegate;
};