// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/ARPlayerState.h"
#include "AbilitySystemComponent.h"

AARPlayerState::AARPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
}

UAbilitySystemComponent* AARPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
