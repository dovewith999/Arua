// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Model/ModelBase.h"
#include "PlayerData.generated.h"

/**
 * Test용 Model
 * 작성자 : 임희섭
 * 작성일 : 25/11/04
 */
UCLASS()
class ARUA_API UPlayerData : public UModelBase
{
	GENERATED_BODY()

public:
	FORCEINLINE float GetHp() const { return Hp; }
	FORCEINLINE float GetMaxHp() const { return MaxHp; }
	FORCEINLINE float GetHpRatio() const { return Hp / MaxHp; }

	FORCEINLINE void TakeDamage(float InDamage) { Hp = FMath::Clamp(Hp - InDamage, 0, MaxHp); }
	FORCEINLINE void SetHp(float InNewHp) { Hp = InNewHp; }

private:
	UPROPERTY()
	float Hp;

	UPROPERTY()
	float MaxHp;
};
