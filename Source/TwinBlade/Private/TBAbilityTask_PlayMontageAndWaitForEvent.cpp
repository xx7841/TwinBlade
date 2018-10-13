// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TBAbilityTask_PlayMontageAndWaitForEvent.h"
#include "AbilitySystemGlobals.h"
#include "Animation/AnimInstance.h"
#include "TBAbilitySystemComponent.h"

UTBAbilityTask_PlayMontageAndWaitForEvent::UTBAbilityTask_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	Rate = 1.0f;
	bIsStopMontageWhenAbilityEnds = true;
}

void UTBAbilityTask_PlayMontageAndWaitForEvent::Activate() {
	if (Ability == nullptr) {
		return;
	}

	bool bIsPlayMontage = false;
	UTBAbilitySystemComponent* AbilitySystemComponent = GetTargetAbilitySystemComponent();
	if (AbilitySystemComponent) {
		const FGameplayAbilityActorInfo* OwningActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = OwningActorInfo->GetAnimInstance();
		if (AnimInstance != nullptr) {
			EventHandle = AbilitySystemComponent->AddGameplayEventTagContainerDelegate(EventTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UTBAbilityTask_PlayMontageAndWaitForEvent::OnGameplayEvent));

			if (AbilitySystemComponent->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay, Rate, StartSection) > 0.0f) {
				if (ShouldBroadcastAbilityTaskDelegates() == false) {
					return;
				}

				CancelledHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UTBAbilityTask_PlayMontageAndWaitForEvent::OnAbilityCancelled);

				BlendingOutDelegate.BindUObject(this, &UTBAbilityTask_PlayMontageAndWaitForEvent::OnMontageBlendingOut);
				AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

				MontageEndedDelegate.BindUObject(this, &UTBAbilityTask_PlayMontageAndWaitForEvent::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

				ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
				if (Character && (Character->Role == ROLE_Authority || (Character->Role == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted))) {
					Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
				}
				bIsPlayMontage = true;
			}
		}
	}

	if (!bIsPlayMontage) {
		if (ShouldBroadcastAbilityTaskDelegates()) {
			OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}

	SetWaitingOnAvatar();
}

void UTBAbilityTask_PlayMontageAndWaitForEvent::ExternalCancel() {
	check(AbilitySystemComponent);

	OnAbilityCancelled();

	Super::ExternalCancel();
}

void UTBAbilityTask_PlayMontageAndWaitForEvent::OnDestroy(bool bIsAbilityEnd) {
	if (Ability) {
		Ability->OnGameplayAbilityCancelled.Remove(CancelledHandle);
		if (bIsAbilityEnd && bIsStopMontageWhenAbilityEnds) {
			StopPlayingMontage();
		}
	}

	UTBAbilitySystemComponent* AbilitySystemComponent = GetTargetAbilitySystemComponent();
	if (AbilitySystemComponent) {
		AbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(EventTags, EventHandle);
	}

	Super::OnDestroy(bIsAbilityEnd);
}

UTBAbilityTask_PlayMontageAndWaitForEvent* UTBAbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay, FGameplayTagContainer EventTags, float Rate, FName StartSection, bool bIsStopMontageWhenAbilityEnds, float AnimRootMotionTranslationScale) {
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);

	UTBAbilityTask_PlayMontageAndWaitForEvent* Obj = NewAbilityTask<UTBAbilityTask_PlayMontageAndWaitForEvent>(OwningAbility, TaskInstanceName);
	Obj->MontageToPlay = MontageToPlay;
	Obj->EventTags = EventTags;
	Obj->Rate = Rate;
	Obj->StartSection = StartSection;
	Obj->bIsStopMontageWhenAbilityEnds = bIsStopMontageWhenAbilityEnds;
	Obj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;

	return Obj;
}

void UTBAbilityTask_PlayMontageAndWaitForEvent::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload) {
	if (ShouldBroadcastAbilityTaskDelegates()) {
		FGameplayEventData TempData = *Payload;
		TempData.EventTag = EventTag;

		EventReceived.Broadcast(EventTag, TempData);
	}
}

void UTBAbilityTask_PlayMontageAndWaitForEvent::OnAbilityCancelled() {
	if (StopPlayingMontage()) {
		if (ShouldBroadcastAbilityTaskDelegates()) {
			OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

void UTBAbilityTask_PlayMontageAndWaitForEvent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted) {
	if (Ability && Ability->GetCurrentMontage() == MontageToPlay) {
		if (Montage == MontageToPlay) {
			AbilitySystemComponent->ClearAnimatingAbility(Ability);

			ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
			if (Character && (Character->Role == ROLE_Authority || (Character->Role == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted))) {
				Character->SetAnimRootMotionTranslationScale(1.f);
			}
		}
	}

	if (bInterrupted) {
		if (ShouldBroadcastAbilityTaskDelegates()) {
			OnInterrupted.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
	else {
		if (ShouldBroadcastAbilityTaskDelegates()) {
			OnBlendOut.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

void UTBAbilityTask_PlayMontageAndWaitForEvent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
	if (!bInterrupted) {
		if (ShouldBroadcastAbilityTaskDelegates()) {
			OnCompleted.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}

	EndTask();
}

bool UTBAbilityTask_PlayMontageAndWaitForEvent::StopPlayingMontage() {
	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if (!ActorInfo) {
		return false;
	}

	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	if (AnimInstance == nullptr) {
		return false;
	}

	if (AbilitySystemComponent && Ability) {
		if (AbilitySystemComponent->GetAnimatingAbility() == Ability && AbilitySystemComponent->GetCurrentMontage() == MontageToPlay) {
			FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
			if (MontageInstance) {
				MontageInstance->OnMontageBlendingOutStarted.Unbind();
				MontageInstance->OnMontageEnded.Unbind();
			}

			AbilitySystemComponent->CurrentMontageStop();
			return true;
		}
	}
	return false;
}

UTBAbilitySystemComponent* UTBAbilityTask_PlayMontageAndWaitForEvent::GetTargetAbilitySystemComponent() {
	return Cast<UTBAbilitySystemComponent>(AbilitySystemComponent);
}