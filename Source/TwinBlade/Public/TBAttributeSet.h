// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "TwinBlade.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "TBAttributeSet.generated.h"

/* ������ ��ũ�ε��Դϴ�. */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class TWINBLADE_API UTBAttributeSet : public UAttributeSet {
	GENERATED_BODY()
	
public:
	UTBAttributeSet();

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UTBAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UTBAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackPower)
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UTBAttributeSet, AttackPower)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DefensePower)
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UTBAttributeSet, DefensePower)

	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos))
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UTBAttributeSet, Damage)

	/* Effect�� ����Ǹ� Atrribute�� ���� �� ���̶� ����ϰ� �ڵ����� ȣ��Ǵ� �Լ��Դϴ�.*/
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	virtual void OnRep_MaxHealth();

	UFUNCTION()
	virtual void OnRep_Health();

	UFUNCTION()
	virtual void OnRep_AttackPower();

	UFUNCTION()
	virtual void OnRep_DefensePower();
};