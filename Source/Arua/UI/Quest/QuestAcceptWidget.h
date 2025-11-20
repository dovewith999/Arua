// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataTables/QuestData.h"
#include "QuestAcceptWidget.generated.h"

// 수락 창 동작 모드: 새로 수락 / 완료(반납)
UENUM(BlueprintType)
enum class EQuestAcceptMode : uint8
{
	Accept,   // 새로 수락
	TurnIn    // 완료 반납(보상 받기)
};

// 퀘스트 [수락] / [닫기] / [반납] 이벤트 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestAccept, FName, QuestID);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestClose);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestTurnIn, FName, QuestID);

/**
 * 퀘스트 창 위젯 클래스
 * 퀘스트의 상세 내용과 퀘스트 수락/완료 이벤트를 전달
 */
UCLASS()
class ARUA_API UQuestAcceptWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// 퀘스트 데이터 설정 함수
	UFUNCTION(BlueprintCallable)
	void SetQuestInfo(const FQuestData& InQuestData, EQuestAcceptMode InMode);

	// 퀘스트 수락(또는 보상 받기) 버튼
	UFUNCTION()
	void HandleAcceptOrTurnIn();

	// 퀘스트 닫기 콜백 함수
	UFUNCTION()
	void HandleClose();

public:
	// 현재 표시 중인 퀘스트 데이터
	UPROPERTY(BlueprintReadOnly)
	FQuestData CurrentQuestData;

	// 현재 표시 중인 퀘스트 모드
	UPROPERTY(BlueprintReadOnly)
	EQuestAcceptMode CurrentMode = EQuestAcceptMode::Accept;

	// 퀘스트 수락 델리게이트 객체
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestAccept OnQuestAccept;

	// 퀘스트 반납 델리게이트 객체
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestTurnIn OnQuestTurnIn;

	// 퀘스트 닫기 델리게이트 객체
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestClose OnQuestClose;

protected:
	// 퀘스트 타이틀
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> QuestTitleText;

	// 퀘스트 목표
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> QuestObjectiveText;

	// 퀘스트 설명(내용)
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> QuestDescriptionText;

	// 퀘스트 보상
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> QuestRewardText;

	// 수락 버튼
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UButton> AcceptButton;

	// 수락 버튼 라벨 (수락 or 보상 수령)
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> AcceptButtonLabel;

	// 닫기 버튼
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UButton> CloseButton;
};
