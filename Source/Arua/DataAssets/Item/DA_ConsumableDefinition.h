// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/Item/DA_ItemDefinition.h"
#include "GameplayTagContainer.h"
#include "DA_ConsumableDefinition.generated.h"

/**
 * 소모품 아이템이 적용할 게임플레이 이펙트 구조체
 */
USTRUCT(BlueprintType)
struct FConsumableGameplayEffect
{
	GENERATED_BODY()

	// 적용할 GameplayEffect 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Consumable")
	TSubclassOf<class UGameplayEffect> GameplayEffectClass;

	// 적용 수치
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Consumable")
	float EffectAmount = 10.f;

	// SetByCaller 게임 플레이 태그
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Consumable")
	FGameplayTag SetByCallerTag;
};

/**
 * 소모품 아이템 정의 클래스
 */
UCLASS()
class ARUA_API UDA_ConsumableDefinition : public UDA_ItemDefinition
{
	GENERATED_BODY()

public:
	// 퀘스트 아이템 사용 효과
	virtual void ApplyEffect_Implementation(AActor* User) override;

	// 소모품 효과 게임플레이 이펙트 (복수 적용 가능)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Consumable")
	TArray<FConsumableGameplayEffect> ConsumableGameplayEffects;
};
