// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Item/DA_ConsumableDefinition.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

#include "Tag/AruaGameplayTags.h"

void UDA_ConsumableDefinition::ApplyEffect_Implementation(AActor* User)
{
	// 소모품 아이템 사용 시, 플레이어에게 소모품 게임플레이 이펙트 적용
	if (UAbilitySystemComponent* OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(User))
	{
		// 이펙트 콘텍스트 생성
		FGameplayEffectContextHandle EffectContext = OwnerASC->MakeEffectContext();
		EffectContext.AddSourceObject(User);

		// 소모품 효과 게임플레이 이펙트 적용
		for (auto& Effect : ConsumableGameplayEffects)
		{
			// 이펙트 스펙 핸들 생성
			FGameplayEffectSpecHandle SpecHandle = OwnerASC->MakeOutgoingSpec(Effect.GameplayEffectClass, 1, EffectContext);

			if (SpecHandle.IsValid())
			{
				FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
				if (Spec)
				{
					// SetByCaller 태그에 Effect 적용 수치 적용
					if (Effect.SetByCallerTag.IsValid())
					{
						// 체력 변경의 경우, 1초에 걸쳐 적용됨을 고려하여 100으로 나눠 적용
						Spec->SetSetByCallerMagnitude(Effect.SetByCallerTag,
							Effect.SetByCallerTag == AruaGamePlayTags::SetByCaller_Health ?
							Effect.EffectAmount / 100 : Effect.EffectAmount);
					}
				}

				// 게임플레이 이펙트 적용
				FActiveGameplayEffectHandle ActiveGEHandle = OwnerASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
}
