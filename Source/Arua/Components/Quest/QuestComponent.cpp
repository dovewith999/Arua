// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Quest/QuestComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"

UQuestComponent::UQuestComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UQuestComponent::AcceptQuest(const FQuestData& Quest)
{
	// 수락받은 퀘스트가 이미 진행중이거나 완료된 경우 리턴 (오류 방지)
	if (CompletedQuests.Contains(Quest.QuestID) || ActiveQuests.Contains(Quest.QuestID))
	{
		return false;
	}

	// 새 퀘스트 진행 구조체 생성
	FActiveQuest NewQuest;

	// 새로 진행할 퀘스트 데이터 초기화
	NewQuest.QuestData = Quest;

	// 진행중인 퀘스트 목록에 추가
	ActiveQuests.Add(Quest.QuestID, NewQuest);

	// 수락한 퀘스트 로그 출력
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green,
		FString::Printf(TEXT("Accepted: %s"), *Quest.Title.ToString()));

	return true;
}

void UQuestComponent::AddProgressByEvent(EQuestType Type, FName Target, int32 Amount)
{
	// 진행중인 퀘스트 목록 순회
	for (auto& ActiveQuest : ActiveQuests)
	{
		// 진행도를 갱신할 퀘스트, 해당 퀘스트 데이터를 가져옴
		FActiveQuest& AQ = ActiveQuest.Value;
		const FQuestData& AQData = AQ.QuestData;

		// 이미 퀘스트가 완료된 경우 or 퀘스트 타입이 다른 경우 or 데이터가 없거나 타깃이 아닌 경우
		if (AQ.bCompleted ||
			AQData.Type != Type ||
			AQData.ObjectiveTarget != NAME_None &&
			AQData.ObjectiveTarget != Target) continue;

		// 퀘스트의 목표 수치를 갱신
		AQ.CurrentCount = FMath::Clamp(AQ.CurrentCount + Amount, 0, AQData.ObjectiveCount);

		// 퀘스트 진행도 갱신 델리게이트 브로드캐스트
		OnQuestProgress.Broadcast(AQData.QuestID, AQ.CurrentCount);

		// 퀘스트 목표 수치를 만족하는 경우 (퀘스트 완료)
		if (AQ.CurrentCount >= AQData.ObjectiveCount && AQData.ObjectiveCount > 0)
		{
			// 퀘스트 완료 처리
			AQ.bCompleted = true;

			// 퀘스트 보상 지급
			GrantReward(AQData);

			// 완료된 퀘스트 목록에 추가
			CompletedQuests.Add(AQData.QuestID);

			// 퀘스트 완료 델리게이트 브로드캐스트
			OnQuestCompleted.Broadcast(AQData.QuestID);
		}
	}

	// 완료된 퀘스트는 목록에서 제거
	TArray<FName> RemoveKeys;
	for (auto& ActiveQuest : ActiveQuests)
	{
		if (ActiveQuest.Value.bCompleted)
		{
			RemoveKeys.Add(ActiveQuest.Key);
		}
	}

	for (FName Key : RemoveKeys)
	{
		ActiveQuests.Remove(Key);
	}
}

void UQuestComponent::TryCompleteQuest(FName QuestID)
{
	// 완료할 퀘스트를 가져옴
	if (FActiveQuest* AQ = ActiveQuests.Find(QuestID))
	{
		if (!AQ->bCompleted && AQ->CurrentCount >= AQ->QuestData.ObjectiveCount)
		{
			// 퀘스트 완료 처리
			AQ->bCompleted = true;
			GrantReward(AQ->QuestData);
			CompletedQuests.Add(QuestID);
			OnQuestCompleted.Broadcast(QuestID);
			ActiveQuests.Remove(QuestID);
		}
	}
}

void UQuestComponent::GrantReward(const FQuestData& Quest)
{
	// TODO 김준형: 실제 인벤토리/골드 시스템과 보상 연동
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow,
		FString::Printf(TEXT("Reward: Gold %d (%s)"), Quest.RewardGold, *Quest.Title.ToString()));
}