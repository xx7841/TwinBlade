// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TwinBladeCharacter.h"
#include "TBAbility.h"
#include "TBAttributeSet.h"
#include "TBAbilitySystemComponent.h"

ATwinBladeCharacter::ATwinBladeCharacter() {
	// AbilitySystem을 추가하고 Replicate합니다.
	AbilitySystemComponent = CreateDefaultSubobject<UTBAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// AttributeSet을 추가합니다. 다만 Replicate를 명시하지는 않습니다.(Default)
	AttributeSet = CreateDefaultSubobject<UTBAttributeSet>(TEXT("AttributeSet"));

	// AbilitySytem을 초기화하지는 않았습니다.
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
	// 반드시 AbilitySystem이 먼저 추가되어있어야 합니다.
	check(AbilitySystemComponent);

	// Actor의 소유자가 Server여야하며 이전에 AbilitySystem을 초기화한적이 없어야 합니다.
	if (Role == ROLE_Authority && !bAbilitiesInitialized) {
		// Character에 추기한 Ability를 AbilitySpec로 만들고 이를 저장합니다.
		for (TSubclassOf<UTBAbility>& StartupAbility : GameplayAbilities) {
			// Free Input 설정을 위해 특정 입력을 지정하지는 않습니다.(INDEX_NONE)
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, CharacterLevel, INDEX_NONE, this));
		}

		// Passive Ability는 Effect만 즉시 적용하면 됩니다.
		for (TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects) {
			// EffectContextHandle을 만듭니다.
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			// EffectContextHandle에서 Effect가 적용될 대상을 지정합니다.(This)
			EffectContext.AddSourceObject(this);

			// Effect를 EffectSpec으로 만듭니다.
			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, CharacterLevel, EffectContext);
			// 정상적으로 만들어졌다면 Effect를 대상(this)에게 적용합니다.
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