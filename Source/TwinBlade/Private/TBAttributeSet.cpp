// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TBAttributeSet.h"
#include "GameplayEffect.h"
#include "TwinBladeCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "TBAbilitySystemComponent.h"

UTBAttributeSet::UTBAttributeSet()
	: MaxHealth(1.0f), Health(1.0f), AttackPower(0.0f), DefensePower(1.0f), Damage(0.0f) 
{}

void UTBAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& EffectData) {
	Super::PostGameplayEffectExecute(EffectData);

	// EffectContextHandle을 가져옵니다.
	FGameplayEffectContextHandle EffectContext = EffectData.EffectSpec.GetContext();
	// Effect의 가해자의 AbilitySystemComponent를 구합니다.
	UAbilitySystemComponent* CauserASC = EffectContext.GetOriginalInstigatorAbilitySystemComponent();
	// Effect의 가해자가 설정한 Tags 구합니다.
	const FGameplayTagContainer& EffectTags = *EffectData.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	float DeltaValue = 0;
	// Effect의 내용이 Atrribute에 '추가'하는 것이라면,
	if (EffectData.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive) {
		// 추가하는 값을 DeltaValue에 저장합니다.
		DeltaValue = EffectData.EvaluatedData.Magnitude;
	}

	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	ATwinBladeCharacter* TargetCharacter = nullptr;
	// Effect의 Target이 제대로 설정되어 있다면,
	if (EffectData.Target.AbilityActorInfo.IsValid() && EffectData.Target.AbilityActorInfo->AvatarActor.IsValid()) {
		// Target에 대한 정보를 구합니다.
		TargetActor = EffectData.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = EffectData.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<ATwinBladeCharacter>(TargetActor);
	}

	// Effect가 변경할 Atrribute가 Damage라면,
	if (EffectData.EvaluatedData.Attribute == GetDamageAttribute()) {
		AActor* CauserActor = nullptr;
		AController* CauserController = nullptr;
		ATwinBladeCharacter* CasuerCharacter = nullptr;
		// 가해자에 대한 정보가 제대로 설정되어 있다면,
		if (CauserASC && CauserASC->AbilityActorInfo.IsValid() && CauserASC->AbilityActorInfo->AvatarActor.IsValid()) {
			// 가해자에 대한 정보를 구합니다.
			CauserActor = CauserASC->AbilityActorInfo->AvatarActor.Get();
			CauserController = CauserASC->AbilityActorInfo->PlayerController.Get();
			if (CauserController == nullptr && CauserActor != nullptr) {
				if (APawn* Pawn = Cast<APawn>(CauserActor)) {
					CauserController = Pawn->GetController();
				}
			}

			if (CauserController) {
				CasuerCharacter = Cast<ATwinBladeCharacter>(CauserController->GetPawn());
			}
			else {
				CasuerCharacter = Cast<ATwinBladeCharacter>(CauserActor);
			}

			if (EffectContext.GetEffectCauser()) {
				CauserActor = EffectContext.GetEffectCauser();
			}
		}

		// HitResult를 가져옵니다.
		FHitResult HitResult;
		if (EffectContext.GetHitResult()) {
			HitResult = *EffectContext.GetHitResult();
		}

		// 받은 Damage를 가져옵니다.
		const float LocalDamageDone = GetDamage();
		// Damage를 0으로 초기화합니다.
		SetDamage(0.0f);

		// 유효한 Damage를 받았다면,
		if (LocalDamageDone > 0) {
			// 현재 Health를 구합니다.
			const float OldHealth = GetHealth();
			// 갱신된 Health = 현재 Health - Damage.
			SetHealth(FMath::Clamp(OldHealth - LocalDamageDone, 0.0f, GetMaxHealth()));

			// 피해자가 Damage를 받았을 때 처리할 함수를 호출합니다.
			if (TargetCharacter) {
				TargetCharacter->HandleDamage(LocalDamageDone, HitResult, EffectTags, CasuerCharacter, CauserActor);
				TargetCharacter->HandleHealthChanged(-LocalDamageDone, EffectTags);
			}
		}
	}
	// Effect가 변경할 Atrribute가 Health라면,
	else if (EffectData.EvaluatedData.Attribute == GetHealthAttribute()) {
		// 최대 체력보단 높지 않도록 Health를 조정합니다.
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

		if (TargetCharacter) {
			TargetCharacter->HandleHealthChanged(DeltaValue, EffectTags);
		}
	}
}

void UTBAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTBAttributeSet, MaxHealth);
	DOREPLIFETIME(UTBAttributeSet, Health);
	DOREPLIFETIME(UTBAttributeSet, AttackPower);
	DOREPLIFETIME(UTBAttributeSet, DefensePower);
}

void UTBAttributeSet::OnRep_MaxHealth() {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTBAttributeSet, MaxHealth);
}

void UTBAttributeSet::OnRep_Health() {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTBAttributeSet, Health);
}

void UTBAttributeSet::OnRep_AttackPower() {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTBAttributeSet, AttackPower);
}

void UTBAttributeSet::OnRep_DefensePower() {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTBAttributeSet, DefensePower);
}