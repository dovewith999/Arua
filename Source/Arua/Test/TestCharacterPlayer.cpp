// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"

ATestCharacterPlayer::ATestCharacterPlayer()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
}

UAbilitySystemComponent* ATestCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void ATestCharacterPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ASC->InitAbilityActorInfo(this, this);
}

void ATestCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
}
