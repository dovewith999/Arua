// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAttributeSetBase.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemGlobals.h"
#include "Character/ARCharacterBase.h"
#include "AbilitySystemBlueprintLibrary.h"

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
            if (AActor* OwnerActor = GetOwningActor())
            {
                // AbilitySystemComponent에 이벤트 전달
                if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerActor))
                {
                    FGameplayEventData EventData;

                    if (AActor* KillerActor = Data.EffectSpec.GetContext().GetInstigator())
                    {
                        if (UAbilitySystemComponent* KillerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(KillerActor))
                        {
                            FGameplayEventData KillEventData;
                            KillEventData.Instigator = KillerActor;
                            KillEventData.Target = OwnerActor;

                            KillerASC->HandleGameplayEvent(
                                FGameplayTag::RequestGameplayTag(TEXT("Event.Quest.Update")),
                                &KillEventData
                            );
                        }
                    }

                    ASC->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("Event.Dead"), &EventData);
                }
            }
        }
    }
}
