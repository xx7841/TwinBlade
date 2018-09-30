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

	// EffectContextHandle�� �����ɴϴ�.
	FGameplayEffectContextHandle EffectContext = EffectData.EffectSpec.GetContext();
	// Effect�� �������� AbilitySystemComponent�� ���մϴ�.
	UAbilitySystemComponent* CauserASC = EffectContext.GetOriginalInstigatorAbilitySystemComponent();
	// Effect�� �����ڰ� ������ Tags ���մϴ�.
	const FGameplayTagContainer& EffectTags = *EffectData.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	float DeltaValue = 0;
	// Effect�� ������ Atrribute�� '�߰�'�ϴ� ���̶��,
	if (EffectData.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive) {
		// �߰��ϴ� ���� DeltaValue�� �����մϴ�.
		DeltaValue = EffectData.EvaluatedData.Magnitude;
	}

	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	ATwinBladeCharacter* TargetCharacter = nullptr;
	// Effect�� Target�� ����� �����Ǿ� �ִٸ�,
	if (EffectData.Target.AbilityActorInfo.IsValid() && EffectData.Target.AbilityActorInfo->AvatarActor.IsValid()) {
		// Target�� ���� ������ ���մϴ�.
		TargetActor = EffectData.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = EffectData.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<ATwinBladeCharacter>(TargetActor);
	}

	// Effect�� ������ Atrribute�� Damage���,
	if (EffectData.EvaluatedData.Attribute == GetDamageAttribute()) {
		AActor* CauserActor = nullptr;
		AController* CauserController = nullptr;
		ATwinBladeCharacter* CasuerCharacter = nullptr;
		// �����ڿ� ���� ������ ����� �����Ǿ� �ִٸ�,
		if (CauserASC && CauserASC->AbilityActorInfo.IsValid() && CauserASC->AbilityActorInfo->AvatarActor.IsValid()) {
			// �����ڿ� ���� ������ ���մϴ�.
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

		// HitResult�� �����ɴϴ�.
		FHitResult HitResult;
		if (EffectContext.GetHitResult()) {
			HitResult = *EffectContext.GetHitResult();
		}

		// ���� Damage�� �����ɴϴ�.
		const float LocalDamageDone = GetDamage();
		// Damage�� 0���� �ʱ�ȭ�մϴ�.
		SetDamage(0.0f);

		// ��ȿ�� Damage�� �޾Ҵٸ�,
		if (LocalDamageDone > 0) {
			// ���� Health�� ���մϴ�.
			const float OldHealth = GetHealth();
			// ���ŵ� Health = ���� Health - Damage.
			SetHealth(FMath::Clamp(OldHealth - LocalDamageDone, 0.0f, GetMaxHealth()));

			// �����ڰ� Damage�� �޾��� �� ó���� �Լ��� ȣ���մϴ�.
			if (TargetCharacter) {
				TargetCharacter->HandleDamage(LocalDamageDone, HitResult, EffectTags, CasuerCharacter, CauserActor);
				TargetCharacter->HandleHealthChanged(-LocalDamageDone, EffectTags);
			}
		}
	}
	// Effect�� ������ Atrribute�� Health���,
	else if (EffectData.EvaluatedData.Attribute == GetHealthAttribute()) {
		// �ִ� ü�º��� ���� �ʵ��� Health�� �����մϴ�.
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