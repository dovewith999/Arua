// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerData.h"
#include "AttributeSet/PlayerAttributeSet.h"
#include "BossData.h"

void UPlayerData::BindToAttributeSet(UPlayerAttributeSet* InAttributeSet)
{
	if (AttributeSet == InAttributeSet || InAttributeSet == nullptr)
	{
		return;
	}

	AttributeSet = InAttributeSet;
}

float UPlayerData::GetHp() const
{
	return AttributeSet->GetHealth();
}
float UPlayerData::GetMaxHp() const
{
	return AttributeSet->GetMaxHealth();
}

float UPlayerData::GetHpRatio() const
{
	return GetHp() / GetMaxHp();
}

void UPlayerData::TakeDamage(float InDamage)
{
	AttributeSet->SetHealth(InDamage);
}

void UPlayerData::SetHp(float InNewHp)
{
	AttributeSet->SetHealth(InNewHp);
}
void UPlayerData::SetMaxHp(float InNewMaxHp)
{
	AttributeSet->SetMaxHealth(InNewMaxHp);
}

