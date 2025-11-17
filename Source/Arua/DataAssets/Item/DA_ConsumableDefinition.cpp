// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Item/DA_ConsumableDefinition.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

void UDA_ConsumableDefinition::ApplyEffect_Implementation(AActor* User)
{
	// 소모품 아이템 사용 시, 플레이어에게 소모품 게임플레이 이펙트 적용
	if (UAbilitySystemComponent* OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(User))
	{
		FGameplayEffectContextHandle EffectContext = OwnerASC->MakeEffectContext();
		EffectContext.AddSourceObject(User);

		FGameplayEffectSpecHandle SpecHandle = OwnerASC->MakeOutgoingSpec(ConsumableGameplayEffect, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = OwnerASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}
