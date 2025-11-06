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
