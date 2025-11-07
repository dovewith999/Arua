// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataTables/QuestData.h"
#include "QuestButtonWidget.generated.h"

// 퀘스트 버튼 클릭 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestButtonClicked, FName, QuestID);

UCLASS()
class ARUA_API UQuestButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(const FQuestData& InQuestData, bool IsCompletedQuest);

	// 버튼 클릭 함수
	UFUNCTION()
	void HandleOnClicked();

public:
	// 퀘스트 버튼 클릭 델리게이트 변수
	FOnQuestButtonClicked OnClicked;

	// 퀘스트 버튼이 가지는 퀘스트 ID
	FName QuestID;

	// 퀘스트 버튼
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	TObjectPtr<class UButton> QuestButton;

	// 퀘스트 타이틀 텍스트
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	TObjectPtr<class UTextBlock> QuestNameText;

	// 수락 퀘스트 아이콘
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UImage> QuestAcceptedIcon;

	// 완료 퀘스트 아이콘
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UImage> QuestTurnedInIcon;
};
