// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Weapon/ARSword.h"

AARSword::AARSword()
{
	this->SetActorEnableCollision(false);
}

void AARSword::InitializeFromData()
{
	Super::InitializeFromData();
}
