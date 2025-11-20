// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/Item/DA_ItemDefinition.h"
#include "DA_EquipmentDefinition.generated.h"

/**
 * 장비 아이템 정의 클래스
 */
UCLASS()
class ARUA_API UDA_EquipmentDefinition : public UDA_ItemDefinition
{
	GENERATED_BODY()

public:
	// 장비 아이템 사용 효과 (장비 착용)
	virtual void ApplyEffect_Implementation(AActor* User) override;

protected:
	// 장착할 무기 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	TSubclassOf<class AARWeaponBase> WeaponDataClass;

	// 장비의 공격력/방어력을 아이템에서 정의?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	int32 AttackPower = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	int32 DefensePower = 0;
};
