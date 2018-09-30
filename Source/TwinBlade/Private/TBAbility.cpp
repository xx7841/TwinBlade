// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TBAbility.h"
#include "TBTargetingSystem.h"
#include "TwinBladeCharacter.h"
#include "TBAbilitySystemComponent.h"

TArray<FActiveGameplayEffectHandle> UTBAbility::ApplyEffectContainer(FGameplayTag Tag, const FGameplayEventData& EventData) {
	// 먼저 Ability가 가진 FTBEffectContainer들 중 적용할 FTBEffectContainer를 Tag를 이용해 결정해야 합니다.
	// 그리고 결정된 FTBEffectContainer를 분석하여 TargetingResults와 EffectSpecs를 구해야 합니다.
	FTBEffectSpecContainer EffectSpecs = GetEffectSpecContainer(Tag, EventData);

	// 아래의 과정을 통해 Ability의 Effects는 TargetingResults에게 적용됩니다.
	TArray<FActiveGameplayEffectHandle> EffectHandles;
	for (const FGameplayEffectSpecHandle& EffectSpecHandle : EffectSpecs.EffectSpecs) {
		EffectHandles.Append(K2_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, EffectSpecs.TargetingResults));
	}
	return EffectHandles;
}

FTBEffectSpecContainer UTBAbility::GetEffectSpecContainer(FGameplayTag Tag, const FGameplayEventData& EventData) {
	// 여러 FTBEffectContainer들을 저장 중이라면 그 중에서 적용할 FTBEffectContainer를 Tag를 이용해 찾습니다.
	FTBEffectContainer* FoundEffectContainer = EffectContainerMap.Find(Tag);
	// 찾았다면,
	if (FoundEffectContainer) {
		FTBEffectSpecContainer Return;
		// 가해자 Actor를 얻습니다.
		AActor* CauserActor = GetOwningActorFromActorInfo();
		// 가해자 Actor를 이용해 가해저 Character를 얻습니다.
		ATwinBladeCharacter* CauserCharacter = Cast<ATwinBladeCharacter>(CauserActor);
		// 가해자 Actor를 이용해 가해자 Actor의 ASC를 얻습니다.
		UTBAbilitySystemComponent* CauserActorASC = UTBAbilitySystemComponent::GetAbilitySystemComponentByActor(CauserActor);

		// ASC를 정상적으로 얻었다는 건 가해자 Actor에 AbilitySystem이 정상적으로 작동되고 있다는 것을 의미합니다.
		if (CauserActorASC) {
			// 만약 FTBEffectContainer에 TargetingSystem이 존재한다면,
			if (FoundEffectContainer->TargetingSystem.Get()) {
				TArray<AActor*> TargetActors;
				TArray<FHitResult> HitResults;
				AActor* AvatarActor = GetAvatarActorFromActorInfo();
				// TargetingSystemm을 가져옵니다.
				const UTBTargetingSystem* TargetingSystem = FoundEffectContainer->TargetingSystem.GetDefaultObject();
				// TargetingSystem에 정의된 Targeting을 적용하여 피해자 Actors나 HitResults를 구합니다.
				TargetingSystem->GetTargets(CauserCharacter, AvatarActor, EventData, HitResults, TargetActors);
				// TagetingResults를 저장합니다.
				Return.SetTargetingResults(HitResults, TargetActors);
			}

			// FTBEffectSpecContainer 내 모든 Effects에 대해...
			for (const TSubclassOf<UGameplayEffect>& Effect : FoundEffectContainer->Effects) {
				// EffectSpec을 작성하고 저장합니다.
				Return.EffectSpecs.Add(MakeOutgoingGameplayEffectSpec(Effect));
			}
		}
		return Return;
	}
	return FTBEffectSpecContainer();
}