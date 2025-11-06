// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerData.h"
#include "AttributeSet/PlayerAttributeSet.h"

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
	if (AttributeSet == nullptr)
	{
		return 0.f;
	}

	return AttributeSet->GetHealth();
}
float UPlayerData::GetMaxHp() const
{
	if (AttributeSet == nullptr)
	{
		return 0.f;
	}

	return AttributeSet->GetMaxHealth();
}

float UPlayerData::GetHpRatio() const
{
	if (AttributeSet == nullptr)
	{
		return 0.f;
	}

	return GetHp() / GetMaxHp();
}

void UPlayerData::TakeDamage(float InDamage)
{
	if (AttributeSet == nullptr)
	{
		return;
	}

	AttributeSet->SetHealth(InDamage);
}

void UPlayerData::SetHp(float InNewHp)
{
	if (AttributeSet == nullptr)
	{
		return;
	}

	AttributeSet->SetHealth(InNewHp);
}
void UPlayerData::SetMaxHp(float InNewMaxHp)
{
	if (AttributeSet == nullptr)
	{
		return;
	}

	AttributeSet->SetMaxHealth(InNewMaxHp);
}

