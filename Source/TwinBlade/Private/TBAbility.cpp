// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TBAbility.h"
#include "TBTargetingSystem.h"
#include "TwinBladeCharacter.h"
#include "TBAbilitySystemComponent.h"

TArray<FActiveGameplayEffectHandle> UTBAbility::ApplyEffectContainer(FGameplayTag Tag, const FGameplayEventData& EventData) {
	// ���� Ability�� ���� FTBEffectContainer�� �� ������ FTBEffectContainer�� Tag�� �̿��� �����ؾ� �մϴ�.
	// �׸��� ������ FTBEffectContainer�� �м��Ͽ� TargetingResults�� EffectSpecs�� ���ؾ� �մϴ�.
	FTBEffectSpecContainer EffectSpecs = GetEffectSpecContainer(Tag, EventData);

	// �Ʒ��� ������ ���� Ability�� Effects�� TargetingResults���� ����˴ϴ�.
	TArray<FActiveGameplayEffectHandle> EffectHandles;
	for (const FGameplayEffectSpecHandle& EffectSpecHandle : EffectSpecs.EffectSpecs) {
		EffectHandles.Append(K2_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, EffectSpecs.TargetingResults));
	}
	return EffectHandles;
}

FTBEffectSpecContainer UTBAbility::GetEffectSpecContainer(FGameplayTag Tag, const FGameplayEventData& EventData) {
	// ���� FTBEffectContainer���� ���� ���̶�� �� �߿��� ������ FTBEffectContainer�� Tag�� �̿��� ã���ϴ�.
	FTBEffectContainer* FoundEffectContainer = EffectContainerMap.Find(Tag);
	// ã�Ҵٸ�,
	if (FoundEffectContainer) {
		FTBEffectSpecContainer Return;
		// ������ Actor�� ����ϴ�.
		AActor* CauserActor = GetOwningActorFromActorInfo();
		// ������ Actor�� �̿��� ������ Character�� ����ϴ�.
		ATwinBladeCharacter* CauserCharacter = Cast<ATwinBladeCharacter>(CauserActor);
		// ������ Actor�� �̿��� ������ Actor�� ASC�� ����ϴ�.
		UTBAbilitySystemComponent* CauserActorASC = UTBAbilitySystemComponent::GetAbilitySystemComponentByActor(CauserActor);

		// ASC�� ���������� ����ٴ� �� ������ Actor�� AbilitySystem�� ���������� �۵��ǰ� �ִٴ� ���� �ǹ��մϴ�.
		if (CauserActorASC) {
			// ���� FTBEffectContainer�� TargetingSystem�� �����Ѵٸ�,
			if (FoundEffectContainer->TargetingSystem.Get()) {
				TArray<AActor*> TargetActors;
				TArray<FHitResult> HitResults;
				AActor* AvatarActor = GetAvatarActorFromActorInfo();
				// TargetingSystemm�� �����ɴϴ�.
				const UTBTargetingSystem* TargetingSystem = FoundEffectContainer->TargetingSystem.GetDefaultObject();
				// TargetingSystem�� ���ǵ� Targeting�� �����Ͽ� ������ Actors�� HitResults�� ���մϴ�.
				TargetingSystem->GetTargets(CauserCharacter, AvatarActor, EventData, HitResults, TargetActors);
				// TagetingResults�� �����մϴ�.
				Return.SetTargetingResults(HitResults, TargetActors);
			}

			// FTBEffectSpecContainer �� ��� Effects�� ����...
			for (const TSubclassOf<UGameplayEffect>& Effect : FoundEffectContainer->Effects) {
				// EffectSpec�� �ۼ��ϰ� �����մϴ�.
				Return.EffectSpecs.Add(MakeOutgoingGameplayEffectSpec(Effect));
			}
		}
		return Return;
	}
	return FTBEffectSpecContainer();
}