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
}

//void UPlayerAttributeSet::SetChargeCount(float InNewVal)
//{
//	float NewVal = FMath::Max(0.f, NewVal); 
//
//	// Attribute Data의 기본값과 현재 값을 모두 설정
//	ChargeCount.SetBaseValue(NewVal);
//	ChargeCount.SetCurrentValue(NewVal);
//}

void UPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	SetChargeCount(FMath::Clamp(GetChargeCount(), 0.f, GetMaxChargeCount()));
}
