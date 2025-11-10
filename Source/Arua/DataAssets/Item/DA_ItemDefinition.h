// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Texture2D.h"
#include "AruaTypes/Arua_EnumTypes.h"
#include "DA_ItemDefinition.generated.h"

/**
 * 게임에서 사용될 아이템의 기본 정의 클래스
 * 소프트 레퍼런스를 사용하여 에셋을 필요할 때 로드한다.
 */

UCLASS()
class ARUA_API UDA_ItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// 아이템 사용 효과 함수
	UFUNCTION(BlueprintNativeEvent, Category = "Item")
	void ApplyEffect(AActor* User);

public:
	// 아이템 표시 이름
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FText ItemName;

	// UI에 표시될 아이콘 (소프트 레퍼런스)
	// 아이콘 텍스처가 실제로 필요해질 때까지 로딩되지 않으므로, 초기 로딩 시간과 메모리 사용량을 절약
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TSoftObjectPtr<UTexture2D> ItemIcon;

	// 아이템 설명
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (MultiLine = true))
	FText ItemDescription;

	// 아이템 분류
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	EAR_ItemCategory ItemCategory;

	// 최대 겹침 수량 (1이면 겹침 불가)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	int32 MaxStackSize = 1;

	// 장비일 경우 공격력/방어력 등 스탯
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (EditCondition = "Category == EItemCategory::Armor"))
	int32 AttackPower = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (EditCondition = "Category == EItemCategory::Armor"))
	int32 DefensePower = 0;
};
