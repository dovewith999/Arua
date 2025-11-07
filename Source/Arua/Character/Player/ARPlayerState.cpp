// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/ARPlayerState.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet/PlayerAttributeSet.h"

AARPlayerState::AARPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AARPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
