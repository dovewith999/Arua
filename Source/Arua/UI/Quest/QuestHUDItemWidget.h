// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataTables/QuestData.h"
#include "QuestHUDItemWidget.generated.h"

/**
 * 현재 진행 중인 퀘스트와 진행 상황을 표시하는 클래스
 * 퀘스트 HUD 컨테이너에 아래로 쌓이며 퀘스트의 정보와 진행 상황을 간소화하여 표시함
 */
UCLASS()
class ARUA_API UQuestHUDItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 퀘스트 항목 초기화 함수
	UFUNCTION(BlueprintCallable)
	void InitializeQuest(const FQuestData& InQuestData, int32 DisplayOrder);

	// 진행도 갱신 함수 (0/2, 1/2...)
	UFUNCTION(BlueprintCallable)
	void UpdateProgress(int32 Current, int32 Target);

	// 퀘스트 순번 갱신 함수 (다른 퀘스트가 완료되었을 때)
	UFUNCTION(BlueprintCallable)
	void UpdateOrderNumber(int32 NewOrder);

	// 목표 달성 시 라벨/아이콘 바꾸기
	UFUNCTION(BlueprintCallable)
	void TurnInQuest();

public:
	// 퀘스트 순번
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> QuestOrderText;

	// 퀘스트 타이틀/제목
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> QuestTitleText;

	// 퀘스트 타이틀/제목 완료 색상
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FSlateColor QuestTitleTextTurnInColor;

	// 목표 텍스트
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> QuestObjectiveText;

	// 진행도 텍스트
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> QuestProgressText;

	// 진행도 텍스트 완료 색상
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FSlateColor QuestProgressTextTurnInColor;

	// 수락 퀘스트 아이콘
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UImage> QuestAcceptedIcon;

	// 완료 퀘스트 아이콘
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UImage> QuestTurnedInIcon;

private:
	// 해당 항목의 퀘스트 ID
	FName QuestID;
};
