#include "BossData.h"
#include "AttributeSet/MonsterAttributeSet.h"

void UBossData::BindToAttributeSet(UMonsterAttributeSet* InAttributeSet)
{
	if (AttributeSet == InAttributeSet)
	{
		return;
	}

	AttributeSet = InAttributeSet;

	UAbilitySystemComponent* ASC = AttributeSet->GetOwningAbilitySystemComponent();
	if (ASC)
	{
		BindAttributeChange(ASC, UMonsterAttributeSet::GetHealthAttribute(),
			FName("Hp"));
		BindAttributeChange(ASC, UMonsterAttributeSet::GetMaxHealthAttribute(),
			FName("MaxHp"));
		BindAttributeChange(ASC, UMonsterAttributeSet::GetAttackAttribute(),
			FName("Attack"));
	}
}

float UBossData::GetHp() const
{
	return AttributeSet->GetHealth();
}
float UBossData::GetMaxHp() const
{
	return AttributeSet->GetMaxHealth();
}

float UBossData::GetHpRatio() const
{
	if (AttributeSet == nullptr)
	{
		return 0.f;
	}

	return GetHp() / GetMaxHp();
}

FString UBossData::GetName() const
{
	return Name;
}

void UBossData::TakeDamage(float InDamage)
{
	AttributeSet->SetHealth(InDamage);
}

void UBossData::SetIsSensed(bool bInSensed)
{
	if (bIsSensed != bInSensed)
	{
		bIsSensed = bInSensed;

		if (bIsSensed)
		{
			OnModelDataChanged.Broadcast(FName(TEXT("IsSensed"))); // ViewModel에 변경 알림
		}
		else
		{
			OnModelDataChanged.Broadcast(FName(TEXT("IsUnsensed"))); // ViewModel에 변경 알림

		}
	}
}

void UBossData::SetHp(float InNewHp)
{
	AttributeSet->SetHealth(InNewHp);
}
void UBossData::SetMaxHp(float InNewMaxHp)
{
	AttributeSet->SetMaxHealth(InNewMaxHp);
}

void UBossData::SetName(const FString& InName)
{
	Name = InName;
}
