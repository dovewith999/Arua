// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/Item/DA_ItemDefinition.h"
#include "GameplayTagContainer.h"
#include "DA_ConsumableDefinition.generated.h"

/**
 * 플레이어 속성과 연동되는 소모품 아이템 정의
 */
UCLASS()
class ARUA_API UDA_ConsumableDefinition : public UDA_ItemDefinition
{
	GENERATED_BODY()
	
public:
	// 퀘스트 아이템 사용 효과
	virtual void ApplyEffect_Implementation(AActor* User) override;

	// 소모품 효과 게임플레이 이펙트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Consumable")
	TSubclassOf<class UGameplayEffect> ConsumableGameplayEffect;

	// 체력 회복량
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Consumable")
	float HealAmount = 50.f;

	// SetByCaller에 사용할 게임플레이 태그
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Consumable")
	FGameplayTag HealAmountTag;
};
