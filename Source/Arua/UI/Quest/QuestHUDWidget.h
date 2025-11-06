// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestHUDWidget.generated.h"

UCLASS()
class ARUA_API UQuestHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 퀘스트 컴포넌트의 진행 상황을 HUD에 반영하는 델리게이트
	UFUNCTION(BlueprintCallable)
	void BindToQuestComponent(class UQuestComponent* InQuestComponent);

protected:
	// 퀘스트 [수락] / [진행] / [반납] / [완료] 이벤트 처리 함수
	UFUNCTION()
	void HandleQuestAccepted(const FQuestData& QuestData);

	UFUNCTION()
	void HandleQuestProgress(FName QuestID, int32 CurrentCount);

	UFUNCTION()
	void HandleQuestReadyToTurnIn(FName QuestID);

	UFUNCTION()
	void HandleQuestTurnedIn(FName QuestID);

protected:
	// 현재 진행 중인 퀘스트 항목들을 표시하는 VerticalBox
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UVerticalBox> ActiveQuestList;

private:
	// 플레이어의 퀘스트 컴포넌트
	UPROPERTY()
	UQuestComponent* QuestComponent = nullptr;

	// 퀘스트 ID별로 생성된 HUD 항목(Row)을 추적 (진행도 갱신용)
	UPROPERTY()
	TMap<FName, class UHorizontalBox*> QuestRows;
};
