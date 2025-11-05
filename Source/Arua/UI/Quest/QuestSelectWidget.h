// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestSelectWidget.generated.h"

UCLASS()
class ARUA_API UQuestSelectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 퀘스트 리스트 초기화 함수
	void InitQuestList(const TArray<FName>& QuestIDs, class UDataTable* InQuestDataTable);

protected:
	// 퀘스트 버튼 위젯 선택(클릭) 콜백 함수
	UFUNCTION()
	void HandleQuestButtonClicked(FName QuestID);

public:
	// 퀘스트 버튼 위젯들이 들어갈 컨테이너
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	TObjectPtr<class UVerticalBox> QuestButtonContainer;

	// 퀘스트 버튼 위젯 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UQuestButtonWidget> QuestButtonWidgetClass;

	// 퀘스트 수락 위젯이 들어갈 컨테이너
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	TObjectPtr<class USizeBox> QuestAcceptContainer;

	// 퀘스트 수락 위젯 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UQuestAcceptWidget> QuestAcceptWidgetClass;

private:
	// 퀘스트 수락 위젯 인스턴스
	UPROPERTY()
	TObjectPtr<class UQuestAcceptWidget> QuestAcceptWidgetInstance;

	// 퀘스트 데이터 테이블
	UPROPERTY()
	class UDataTable* QuestDataTable;
};
