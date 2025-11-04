// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Quest/QuestComponent.h"

UQuestComponent::UQuestComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UQuestComponent::AcceptQuest(const FQuestData& QuestData)
{
	// 이미 활성화중인 퀘스트인 경우, 퀘스트 수락 실패
	if (IsQuestActive(QuestData.QuestID))
	{
		return false;
	}

	// 새 퀘스트 활성화 구조체 생성
	FActiveQuest NewQuest;
	NewQuest.QuestID = QuestData.QuestID;	// 퀘스트 ID 초기화
	NewQuest.Progress = 0;					// 퀘스트 진행도 0으로 초기화
	NewQuest.bCompleted = false;			// 퀘스트 완료 여부 false로 초기화

	// 활성화한 퀘스트 관리 배열에 추가
	ActiveQuests.Add(NewQuest);

	// 퀘스트 수락 델리게이트 브로드캐스트
	OnQuestAccepted.Broadcast(QuestData.QuestID);

	return true;
}

bool UQuestComponent::UpdateQuestProgress(const FName& QuestID, int32 Amount)
{
	// 퀘스트 관리 배열 순회
	for (auto& Quest : ActiveQuests)
	{
		// 동일한 퀘스트 ID이고, 퀘스트가 완료되지 않은 경우
		if (Quest.QuestID == QuestID && !Quest.bCompleted)
		{
			// 퀘스트의 진행도 업데이트
			Quest.Progress += Amount;

			// TODO 김준형: 퀘스트 진행도와 퀘스트 목표 수치 비교 후 완료 처리 구현
			//if (Quest.Progress >= )

			return true;
		}
	}

	return false;
}

void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UQuestComponent::IsQuestActive(const FName& QuestID) const
{
	// 활성화한 퀘스트 관리 배열을 순회
	for (const auto& Quest : ActiveQuests)
	{
		// 동일한 QuestID를 활성화 중인 경우 true 반환
		if (Quest.QuestID == QuestID)
		{
			return true;
		}
	}

	return false;
}
