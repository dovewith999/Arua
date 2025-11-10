// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ARCharacterPlayer.h"
#include "HSTestCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API AHSTestCharacterPlayer : public AARCharacterPlayer
{
	GENERATED_BODY()
	
public:
	AHSTestCharacterPlayer();

	//FORCEINLINE AARWeaponBase* GetWeapon() { return CurrentWeapon; }

};
