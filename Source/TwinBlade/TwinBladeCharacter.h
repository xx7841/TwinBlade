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
class ATwinBladeCharacter : public ACharacter, public IAbilitySystemInterface
{
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
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool ActivateAbilityByClass(TSubclassOf<UTBAbility> Ability, bool bAllowRemoteActivation = true);

	/* */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UTBAbility*>& ActiveAbilities);

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UTBAbility>> GameplayAbilities;

	/* */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	/* */
	UPROPERTY(EditAnywhere, Replicated, Category = Abilities)
	int32 CharacterLevel;

	/* */
	friend UTBAttributeSet;
};