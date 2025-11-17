// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/Item/DA_ItemDefinition.h"
#include "DA_QuestItemDefinition.generated.h"

/**
 * 퀘스트 진행과 연동되는 퀘스트 아이템 정의
 */
UCLASS()
class ARUA_API UDA_QuestItemDefinition : public UDA_ItemDefinition
{
	GENERATED_BODY()

public:
	// 퀘스트 아이템 사용 효과
	virtual void ApplyEffect_Implementation(AActor* User) override;

	// 이 퀘스트 아이템의 목표 타깃 ID
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	FName ObjectiveTargetID;
};
