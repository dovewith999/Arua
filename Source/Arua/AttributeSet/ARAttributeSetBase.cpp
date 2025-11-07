// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAttributeSetBase.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemGlobals.h"

UARAttributeSetBase::UARAttributeSetBase()
{
}

void UARAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));

        if (GetHealth() <= 0.f)
        {
            // 여기서 죽음 처리
            if (AActor* OwnerActor = GetOwningActor())
            {
                // AbilitySystemComponent에 이벤트 전달
                if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerActor))
                {
                    FGameplayEventData EventData;
                    ASC->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("Event.Dead"), &EventData);
                }
            }
        }
    }
}
