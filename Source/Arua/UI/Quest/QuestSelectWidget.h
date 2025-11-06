// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestSelectWidget.generated.h"

// 퀘스트 수락 델리게이트 (UI 정리 및 퀘스트 액터의 퀘스트 정리)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestAccepted, FName, QuestID);

// 퀘스트 보상 수령 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestTurnInRequested, FName, QuestID);

UCLASS()
class ARUA_API UQuestSelectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 퀘스트 리스트 초기화 함수
	void InitQuestList(APawn* InInteractor, const TArray<FName>& QuestIDs, class UDataTable* InQuestDataTable);

protected:
	// 퀘스트 버튼 위젯 선택(클릭) 콜백 함수
	UFUNCTION()
	void HandleQuestButtonClicked(FName QuestID);

	// 퀘스트 수락 시, UI및 퀘스트 NPC 처리 함수 (해당 퀘스트 목록 제거 및 퀘스트 액터 데이터 갱신)
	UFUNCTION()
	void HandleOnQuestAccepted(FName QuestID);

	// 퀘스트 보상 수령 콜백 함수
	UFUNCTION()
	void HandleOnQuestTurnIn(FName QuestID);

public:
	// 퀘스트 수락 델리게이트 객체
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestAccepted OnQuestAccepted;

	// 퀘스트 보상 수령 델리게이트 객체
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestTurnInRequested OnQuestTurnInRequested;

protected:
	// 퀘스트 버튼 위젯들이 들어갈 컨테이너
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	TObjectPtr<class UVerticalBox> QuestButtonContainer;

	// 퀘스트 버튼 위젯 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UQuestButtonWidget> QuestButtonWidgetClass;

	// 퀘스트 수락/완료 위젯이 들어갈 컨테이너
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	TObjectPtr<class USizeBox> QuestAcceptContainer;

	// 퀘스트 수락/완료 위젯 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UQuestAcceptWidget> QuestAcceptWidgetClass;

private:
	// 퀘스트 수락 위젯 인스턴스
	UPROPERTY()
	TObjectPtr<class UQuestAcceptWidget> QuestAcceptWidgetInstance;

	// 퀘스트 데이터 테이블
	UPROPERTY()
	class UDataTable* QuestDataTable;

	// 수락 가능한 퀘스트 목록
	UPROPERTY()
	TMap<FName, class UQuestButtonWidget*> AcceptButtons;

	// 보상 수령 가능한 퀘스트 목록
	UPROPERTY()
	TMap<FName, class UQuestButtonWidget*> TurnInButtons;
};
