// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataTables/QuestData.h"
#include "QuestAcceptWidget.generated.h"

// 퀘스트 수락 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestAccept, FName, QuestID);

// 퀘스트 닫기 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestClose);

UCLASS()
class ARUA_API UQuestAcceptWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// 퀘스트 데이터 설정 함수
	UFUNCTION(BlueprintCallable)
	void SetQuestInfo(const FQuestData& InQuestData);

	// 퀘스트 수락 콜백 함수
	UFUNCTION()
	void HandleAccept();

	// 퀘스트 닫기 콜백 함수
	UFUNCTION()
	void HandleClose();

public:
	// 현재 표시 중인 퀘스트 데이터
	UPROPERTY(BlueprintReadOnly)
	FQuestData CurrentQuestData;

	// 퀘스트 수락 델리게이트 객체
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestAccept OnQuestAccept;

	// 퀘스트 닫기 델리게이트 객체
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestClose OnQuestClose;

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

	// 닫기 버튼
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UButton> CloseButton;
};
