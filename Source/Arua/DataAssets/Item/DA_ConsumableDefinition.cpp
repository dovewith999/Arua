// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Item/DA_ConsumableDefinition.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

void UDA_ConsumableDefinition::ApplyEffect_Implementation(AActor* User)
{
	// 소모품 아이템 사용 시, 플레이어에게 소모품 게임플레이 이펙트 적용
	if (UAbilitySystemComponent* OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(User))
	{
		// 이펙트 콘텍스트 생성
		FGameplayEffectContextHandle EffectContext = OwnerASC->MakeEffectContext();
		EffectContext.AddSourceObject(User);

		// 이펙트 스펙 핸들 생성
		FGameplayEffectSpecHandle SpecHandle = OwnerASC->MakeOutgoingSpec(ConsumableGameplayEffect, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
			if (Spec)
			{
				// 회복시킬 힐 태그에 해당 아이템의 회복량 적용
				if (HealAmountTag.IsValid())
				{
					Spec->SetSetByCallerMagnitude(HealAmountTag, HealAmount);
				}
			}

			// 게임플레이 이펙트 적용 (힐 적용)
			FActiveGameplayEffectHandle ActiveGEHandle = OwnerASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}
