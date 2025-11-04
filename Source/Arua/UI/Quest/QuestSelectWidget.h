// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestSelectWidget.generated.h"

// 퀘스트 선택 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestSelected, FName, QuestID);

// 퀘스트 선택 취소 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestDeselected);

UCLASS()
class ARUA_API UQuestSelectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 퀘스트 리스트 초기화 함수
	void InitQuestList(const TArray<FName>& QuestIDs);

	// 퀘스트 선택 함수
	UFUNCTION()
	void HandleQuestButtonClicked(FName InQuestID);

public:
	// 퀘스트 선택 델리게이트 변수
	FOnQuestSelected OnQuestSelected;

	// 퀘스트 선택 취소 델리게이트 변수
	FOnQuestDeselected OnQuestDeselected;

	// 퀘스트 버튼 위젯들을 담을 버티컬 박스
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	TObjectPtr<class UVerticalBox> QuestButtonContainer;

	// 퀘스트 버튼 위젯
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UQuestButtonWidget> QuestButtonWidgetClass;
};
