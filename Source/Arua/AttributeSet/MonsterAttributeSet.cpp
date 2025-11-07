// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSet/MonsterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
//#include "AbilitySystemGlobals.h"

UMonsterAttributeSet::UMonsterAttributeSet()
{
    MaxHealth.SetBaseValue(200.f);
    MaxHealth.SetCurrentValue(200.f);

    Health.SetBaseValue(30.f);
    Health.SetCurrentValue(30.f);

    Attack.SetBaseValue(10.f);
    Attack.SetCurrentValue(10.f);
}

void UMonsterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);
}
