// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSet/MonsterAttributeSet.h"

UMonsterAttributeSet::UMonsterAttributeSet()
{
    MaxHealth.SetBaseValue(200.f);
    MaxHealth.SetCurrentValue(200.f);

    Health.SetBaseValue(200.f);
    Health.SetCurrentValue(200.f);
}
