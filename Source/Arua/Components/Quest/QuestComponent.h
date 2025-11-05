// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataTables/QuestData.h"
#include "QuestComponent.generated.h"

// 퀘스트 진행 구조체
USTRUCT(BlueprintType)
struct FActiveQuest
{
	GENERATED_BODY()

	// 진행중인 퀘스트 데이터
	UPROPERTY(BlueprintReadOnly)
	FQuestData QuestData;

	// 퀘스트 진행도 (퀘스트 목표 수치)
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentCount = 0;

	// 퀘스트 완료 여부
	UPROPERTY(BlueprintReadOnly)
	bool bCompleted = false;
};

// 퀘스트 진행도 갱신 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestProgress, FName, QuestID, int32, CurrentCount);

// 퀘스트 완료 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestCompleted, FName, QuestID);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARUA_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestComponent();

	// 퀘스트 수락 함수
	UFUNCTION(BlueprintCallable)
	bool AcceptQuest(const FQuestData& Quest);

	// 퀘스트 진행도 갱신 함수 (타입/타깃/증가량으로 처리)
	UFUNCTION(BlueprintCallable)
	void AddProgressByEvent(EQuestType Type, FName Target, int32 Amount = 1);

	// 직접 완료 체크 함수(옵션)
	UFUNCTION(BlueprintCallable)
	void TryCompleteQuest(FName QuestID);

protected:
	// 보상 지급 함수
	void GrantReward(const FQuestData& Quest);

public:
	// 퀘스트 진행도 갱신 델리게이트 객체
	UPROPERTY(BlueprintAssignable)
	FOnQuestProgress OnQuestProgress;

	// 퀘스트 완료 델리게이트 객체
	UPROPERTY(BlueprintAssignable)
	FOnQuestCompleted OnQuestCompleted;

	// 진행중인 퀘스트 목록 (퀘스트 ID, 퀘스트 데이터)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	TMap<FName, FActiveQuest> ActiveQuests;

	// 완료된 퀘스트 목록 (퀘스트 ID)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	TSet<FName> CompletedQuests;
};
