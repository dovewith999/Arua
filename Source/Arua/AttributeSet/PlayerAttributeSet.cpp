// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAttributeSet.h"

UPlayerAttributeSet::UPlayerAttributeSet()
{
	Attack.SetBaseValue(10.f);
	Attack.SetCurrentValue(10.f);
	ChargeCount.SetBaseValue(0.f);
	ChargeCount.SetCurrentValue(0.f);
	MaxChargeCount.SetBaseValue(2.f);
	MaxChargeCount.SetCurrentValue(2.f);	

	Stemina.SetBaseValue(100.f);
	Stemina.SetCurrentValue(100.f);
	MaxStemina.SetBaseValue(100.f);
	MaxStemina.SetCurrentValue(100.f);
}

void UPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	SetChargeCount(FMath::Clamp(GetChargeCount(), 0.f, GetMaxChargeCount()));
}
