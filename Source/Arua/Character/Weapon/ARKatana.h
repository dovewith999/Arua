// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Weapon/ARWeaponBase.h"
#include "ARKatana.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API AARKatana : public AARWeaponBase
{
	GENERATED_BODY()

public:
	AARKatana();
private:
	virtual void InitializeFromData() override;
	
};
