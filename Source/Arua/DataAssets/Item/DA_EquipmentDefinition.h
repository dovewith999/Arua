// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/Item/DA_ItemDefinition.h"
#include "DA_EquipmentDefinition.generated.h"

/**
 * 플레이어 무기와 연동되는 장비 아이템 정의
 */
UCLASS()
class ARUA_API UDA_EquipmentDefinition : public UDA_ItemDefinition
{
	GENERATED_BODY()

public:
	// 장비 아이템 사용 효과 (장비 착용)
	virtual void ApplyEffect_Implementation(AActor* User) override;

	// 장착할 무기 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	TSubclassOf<class AARWeaponBase> WeaponBaseClass;
};
