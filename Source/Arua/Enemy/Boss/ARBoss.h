// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/ARMonsterBase.h"
#include "ARBoss.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API AARBoss : public AARMonsterBase
{
	GENERATED_BODY()
	
public:
	AARBoss();
	
	//추후에 interface로 리팩토링 예정
	void AttackFireBreathSwipe();
	void AttackPawLeft();

protected:
	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> MontageAttackFireBreathSwipe;

	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> MontageAttackPawLeft;

};
