// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TwinBladeCharacter.h"
#include "TBAbility.h"
#include "TBAttributeSet.h"
#include "TBAbilitySystemComponent.h"

ATwinBladeCharacter::ATwinBladeCharacter() {
	// AbilitySystem�� �߰��ϰ� Replicate�մϴ�.
	AbilitySystemComponent = CreateDefaultSubobject<UTBAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// AttributeSet�� �߰��մϴ�. �ٸ� Replicate�� ��������� �ʽ��ϴ�.(Default)
	AttributeSet = CreateDefaultSubobject<UTBAttributeSet>(TEXT("AttributeSet"));

	// AbilitySytem�� �ʱ�ȭ������ �ʾҽ��ϴ�.
	bAbilitiesInitialized = false;
}

void ATwinBladeCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent) {
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAbilities();
	}
}

void ATwinBladeCharacter::OnRep_Controller() {
	Super::OnRep_Controller();

	if (AbilitySystemComponent) {
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}
}

void ATwinBladeCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATwinBladeCharacter, CharacterLevel);
}

bool ATwinBladeCharacter::ActivateAbilityByClass(TSubclassOf<UTBAbility> Ability, bool bAllowRemoteActivation) {
	if (AbilitySystemComponent) {
		return AbilitySystemComponent->TryActivateAbilityByClass(Ability, bAllowRemoteActivation);
	}

	return false;
}

void ATwinBladeCharacter::GetActiveAbilitiesByTags(FGameplayTagContainer AbilityTags, TArray<UTBAbility*>& ActiveAbilities) {
	if (AbilitySystemComponent) {
		AbilitySystemComponent->GetActiveAbilitiesByTags(AbilityTags, ActiveAbilities);
	}
}

bool ATwinBladeCharacter::GetCoolTimeByTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration) {
	if (AbilitySystemComponent && CooldownTags.Num() > 0) {
		TimeRemaining = 0.0f;
		CooldownDuration = 0.0f;

		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
		TArray<TPair<float, float>> DurationAndTimeRemaining = AbilitySystemComponent->GetActiveEffectsTimeRemainingAndDuration(Query);
		if (DurationAndTimeRemaining.Num() > 0) {
			int32 BestIdx = 0;
			float LongestTime = DurationAndTimeRemaining[0].Key;
			for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx) {
				if (DurationAndTimeRemaining[Idx].Key > LongestTime) {
					LongestTime = DurationAndTimeRemaining[Idx].Key;
					BestIdx = Idx;
				}
			}

			TimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
			CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;

			return true;
		}
	}
	return false;
}

UAbilitySystemComponent* ATwinBladeCharacter::GetAbilitySystemComponent() const {
	return AbilitySystemComponent;
}

float ATwinBladeCharacter::GetMaxHealth() const {
	return AttributeSet->GetMaxHealth();
}

float ATwinBladeCharacter::GetHealth() const {
	return AttributeSet->GetHealth();
}

void ATwinBladeCharacter::InitializeAbilities() {
	// �ݵ�� AbilitySystem�� ���� �߰��Ǿ��־�� �մϴ�.
	check(AbilitySystemComponent);

	// Actor�� �����ڰ� Server�����ϸ� ������ AbilitySystem�� �ʱ�ȭ������ ����� �մϴ�.
	if (Role == ROLE_Authority && !bAbilitiesInitialized) {
		// Character�� �߱��� Ability�� AbilitySpec�� ����� �̸� �����մϴ�.
		for (TSubclassOf<UTBAbility>& StartupAbility : GameplayAbilities) {
			// Free Input ������ ���� Ư�� �Է��� ���������� �ʽ��ϴ�.(INDEX_NONE)
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, CharacterLevel, INDEX_NONE, this));
		}

		// Passive Ability�� Effect�� ��� �����ϸ� �˴ϴ�.
		for (TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects) {
			// EffectContextHandle�� ����ϴ�.
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			// EffectContextHandle���� Effect�� ����� ����� �����մϴ�.(This)
			EffectContext.AddSourceObject(this);

			// Effect�� EffectSpec���� ����ϴ�.
			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, CharacterLevel, EffectContext);
			// ���������� ��������ٸ� Effect�� ���(this)���� �����մϴ�.
			if (NewHandle.IsValid()) {
				AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
			}
		}
		bAbilitiesInitialized = true;
	}
}

void ATwinBladeCharacter::HandleDamage(float DamageValue, const FHitResult& HitResult, const struct FGameplayTagContainer& DamageTags, ATwinBladeCharacter* CauserCharacter, AActor* CauserActor) {
	OnDamaged(DamageValue, HitResult, DamageTags, CauserCharacter, CauserActor);
}

void ATwinBladeCharacter::HandleHealthChanged(float HealthValue, const struct FGameplayTagContainer& HealthTags) {
	if (bAbilitiesInitialized) {
		OnHealthChanged(HealthValue);
	}
}