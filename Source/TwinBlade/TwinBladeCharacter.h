// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "TwinBlade.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "TBAbility.h"
#include "TwinBladeCharacter.generated.h"

class UGameplayEffect;
class UTBAttributeSet;
class UTBAbilitySystemComponent;

UCLASS()
class ATwinBladeCharacter : public ACharacter, public IAbilitySystemInterface {
	GENERATED_BODY()

public:
	ATwinBladeCharacter();

	/* */
	virtual void PossessedBy(AController* NewController) override;

	/* */
	virtual void OnRep_Controller() override;

	/* */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/* */
	UFUNCTION(BlueprintCallable)
	bool ActivateAbilityByClass(TSubclassOf<UTBAbility> Ability, bool bAllowRemoteActivation = true);

	/* */
	UFUNCTION(BlueprintCallable)
	void GetActiveAbilitiesByTags(FGameplayTagContainer AbilityTags, TArray<UTBAbility*>& ActiveAbilities);

	/* */
	UFUNCTION(BlueprintCallable)
	bool GetCoolTimeByTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration);

	/* */
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/* */
	UFUNCTION(BlueprintCallable)
	virtual float GetMaxHealth() const;

	/* */
	UFUNCTION(BlueprintCallable)
	virtual float GetHealth() const;

protected:
	/* */
	void InitializeAbilities();

	/* */
	virtual void HandleDamage(float DamageValue, const FHitResult& HitResult, const struct FGameplayTagContainer& DamageTags, ATwinBladeCharacter* CauserCharacter, AActor* CauserActor);

	/* */
	virtual void HandleHealthChanged(float HealthValue, const struct FGameplayTagContainer& EventTags);

	/* */
	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float DamageValue, const FHitResult& HitResult, const struct FGameplayTagContainer& DamageTags, ATwinBladeCharacter* CauserCharacter, AActor* CauserActor);

	/* */
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float HealthValue);

protected:
	/* */
	UPROPERTY()
	UTBAbilitySystemComponent* AbilitySystemComponent;

	/* */
	UPROPERTY()
	UTBAttributeSet* AttributeSet;

	/* */
	UPROPERTY()
	int32 bAbilitiesInitialized;

	/* */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<UTBAbility>> GameplayAbilities;

	/* */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	/* */
	UPROPERTY(EditAnywhere, Replicated)
	int32 CharacterLevel;

	/* */
	friend UTBAttributeSet;
};