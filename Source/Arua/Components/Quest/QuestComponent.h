// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataTables/QuestData.h"
#include "QuestComponent.generated.h"

// 퀘스트 활성화 구조체
USTRUCT(BlueprintType)
struct FActiveQuest
{
	GENERATED_BODY()

	// 퀘스트 ID
	UPROPERTY(BlueprintReadOnly)
	FName QuestID;

	// 퀘스트 진행도 (퀘스트 목표 수치)
	UPROPERTY(BlueprintReadOnly)
	int32 Progress = 0;

	// 퀘스트 완료 여부
	UPROPERTY(BlueprintReadOnly)
	bool bCompleted = false;
};

// 퀘스트 수락 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestAccepted, const FName&, QuestID);

// 퀘스트 완료 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestCompleted, const FName&, QuestID);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARUA_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestComponent();

	// 퀘스트 수락 함수
	UFUNCTION(BlueprintCallable)
	bool AcceptQuest(const FQuestData& QuestData);

	// 퀘스트 진행도 업데이트 함수 (퀘스트 완료 확인)
	UFUNCTION(BlueprintCallable)
	bool UpdateQuestProgress(const FName& QuestID, int32 Amount = 1);

	// 활성화한 퀘스트 관리 배열 Getter 함수
	UFUNCTION(BlueprintCallable)
	const TArray<FActiveQuest>& GetActiveQuests() const { return ActiveQuests; }

protected:
	virtual void BeginPlay() override;

	// 퀘스트 활성화(진행 중) 여부
	bool IsQuestActive(const FName& QuestID) const;

public:
	// 퀘스트 수락 델리게이트 변수
	UPROPERTY(BlueprintAssignable)
	FOnQuestAccepted OnQuestAccepted;

	// 퀘스트 완료 델리게이트 변수
	UPROPERTY(BlueprintAssignable)
	FOnQuestCompleted OnQuestCompleted;

protected:
	// 활성화한 퀘스트 관리 배열
	UPROPERTY()
	TArray<FActiveQuest> ActiveQuests;

};
