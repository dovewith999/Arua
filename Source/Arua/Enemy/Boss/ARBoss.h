// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ARCharacterBase.h"
#include "ARBoss.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API AARBoss : public AARCharacterBase
{
	GENERATED_BODY()
	
public:
	AARBoss();
	
	//추후에 interface로 리팩토링 예정
	void Attack_FireBreath_Swipe();
	
protected:
	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> Montage_Attack_FireBreath_Swipe;

};
