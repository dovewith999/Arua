// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Weapon/ARWeaponBase.h"
#include "ARSword.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API AARSword : public AARWeaponBase
{
	GENERATED_BODY()

public:
	AARSword();

private:
	virtual void InitializeFromData() override;
	
};
