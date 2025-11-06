// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataTables/QuestData.h"
#include "QuestComponent.generated.h"

// 퀘스트 진행 상태 열거형
UENUM(BlueprintType)
enum class EQuestStatus : uint8
{
	None,			// 수락하지 않은 상태
	InProgress,     // 목표 미달성
	ReadyToTurnIn,  // 목표 달성, NPC에게 반납 필요
	TurnedIn        // 반납 완료(보상 지급됨)
};

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

	// 퀘스트 진행 상태
	UPROPERTY(BlueprintReadOnly)
	EQuestStatus Status = EQuestStatus::None;
};

// 퀘스트 수락 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestAcceptedSig, const FQuestData&, Quest);

// 퀘스트 진행도 갱신 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestProgressSig, FName, QuestID, int32, CurrentCount);

// 퀘스트 목표 달성 및 반납 필요 처리 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestReadyToTurnInSig, FName, QuestID);

// 퀘스트 반납 완료 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestTurnedInSig, FName, QuestID);

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

	// NPC에게 반납(보상 지급 시점)
	UFUNCTION(BlueprintCallable)
	bool TurnInQuest(FName QuestID);

	// 자가 퀘스트 완료 함수(옵션)
	UFUNCTION(BlueprintCallable)
	void TryCompleteQuest(FName QuestID);

	// 퀘스트 진행 상태 반환 함수
	const EQuestStatus GetQuestStatusFromQuestID(const FName QuestID) const;

protected:
	// 보상 지급 함수
	void GrantReward(const FQuestData& Quest);

public:
	// 퀘스트 수락 델리게이트 객체
	UPROPERTY(BlueprintAssignable)
	FOnQuestAcceptedSig OnQuestAcceptedSig;

	// 퀘스트 진행도 갱신 델리게이트 객체
	UPROPERTY(BlueprintAssignable)
	FOnQuestProgressSig OnQuestProgressSig;

	// 퀘스트 목표 달성 및 반납 필요 처리 델리게이트 객체
	UPROPERTY(BlueprintAssignable)
	FOnQuestReadyToTurnInSig OnQuestReadyToTurnInSig;

	// 퀘스트 반납 완료 델리게이트 객체
	UPROPERTY(BlueprintAssignable)
	FOnQuestTurnedInSig OnQuestTurnedInSig;

	// 진행중인 퀘스트 목록 (퀘스트 ID, 퀘스트 데이터)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	TMap<FName, FActiveQuest> ActiveQuests;

	// 완료된 퀘스트 목록 (퀘스트 ID)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	TSet<FName> CompletedQuests;
};
