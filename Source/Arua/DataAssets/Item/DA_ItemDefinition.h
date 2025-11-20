// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Texture2D.h"
#include "AruaTypes/Arua_EnumTypes.h"
#include "DA_ItemDefinition.generated.h"

/**
 * 아이템의 기본 정의 클래스
 */
UCLASS()
class ARUA_API UDA_ItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/**
	* 아이템 사용 효과 함수
	* 파생 클래스에서 재정의하여 구체적인 효과를 구현
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Item")
	void ApplyEffect(AActor* User);
	virtual void ApplyEffect_Implementation(AActor* User);

public:
	// 아이템 표시 이름
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FText ItemName;

	// UI에 표시될 아이콘 (소프트 레퍼런스)
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
};
