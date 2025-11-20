// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Quest/QuestComponent.h"
#include "Components/Inventory/InventoryComponent.h"

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

	// 수락한 퀘스트 로그 출력
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green,
	//	FString::Printf(TEXT("Accepted: %s"), *Quest.Title.ToString()));

	// 새 퀘스트 진행 구조체 생성
	FActiveQuest NewQuest;

	// 퀘스트 데이터 초기화
	NewQuest.QuestData = Quest;

	// 퀘스트 진행도 초기화
	NewQuest.CurrentCount = 0;

	// 퀘스트 진행 상태 초기화
	NewQuest.Status = EQuestStatus::InProgress;

	// 진행중인 퀘스트 목록에 추가
	ActiveQuests.Add(Quest.QuestID, NewQuest);

	// 퀘스트 수락 브로드캐스트
	OnQuestAcceptedSig.Broadcast(Quest);

	return true;
}

void UQuestComponent::AddProgressByEvent(EQuestType Type, FName Target, int32 Amount)
{
	// 진행중인 퀘스트 목록 순회
	for (auto& ActiveQuest : ActiveQuests)
	{
		// 진행도를 갱신할 퀘스트, 해당 퀘스트 데이터를 가져옴
		FActiveQuest& Quest = ActiveQuest.Value;
		const FQuestData& DataData = Quest.QuestData;

		// 이미 반납 대기/완료면 건너뜀
		if (Quest.Status != EQuestStatus::InProgress) continue;

		// 타입/타깃 일치 검사
		if (DataData.Type != Type) continue;
		if (DataData.ObjectiveTarget != NAME_None && DataData.ObjectiveTarget != Target) continue;

		// 진행도 갱신
		Quest.CurrentCount = FMath::Clamp(Quest.CurrentCount + Amount, 0, DataData.ObjectiveCount);
		OnQuestProgressSig.Broadcast(DataData.QuestID, Quest.CurrentCount);

		// 퀘스트 완료 → 반납 대기 상태로 전환 (보상은 아직 X)
		if (Quest.CurrentCount >= DataData.ObjectiveCount && DataData.ObjectiveCount > 0)
		{
			Quest.Status = EQuestStatus::ReadyToTurnIn;

			// HUD/저널에서 [완료 가능]으로 표시
			OnQuestReadyToTurnInSig.Broadcast(DataData.QuestID);
		}
	}
}

bool UQuestComponent::TurnInQuest(FName QuestID)
{
	// 반납할 퀘스트 가져오기
	FActiveQuest* Quest = ActiveQuests.Find(QuestID);
	if (!Quest)
	{
		return false;
	}

	// 반납 가능한 상태인지 확인
	if (Quest->Status != EQuestStatus::ReadyToTurnIn)
	{
		return false;
	}

	// 보상 지급 시점
	GrantReward(Quest->QuestData);

	// 퀘스트 진행 상태 갱신
	Quest->Status = EQuestStatus::TurnedIn;

	// Active 목록에서 제거
	ActiveQuests.Remove(QuestID);

	// 완료된 퀘스트 목록에 추가
	CompletedQuests.Add(QuestID);

	// UI 반영 브로드캐스트
	OnQuestTurnedInSig.Broadcast(QuestID);

	return true;
}

void UQuestComponent::TryCompleteQuest(FName QuestID)
{
	// 완료할 퀘스트를 가져옴
	if (FActiveQuest* Quest = ActiveQuests.Find(QuestID))
	{
		// 반납 가능한 상태인지 확인
		if (Quest->Status != EQuestStatus::ReadyToTurnIn)
		{
			return;
		}

		// 퀘스트 진행 상태 갱신
		Quest->Status = EQuestStatus::TurnedIn;

		// 보상 지급 시점
		GrantReward(Quest->QuestData);

		// Active 목록에서 제거
		ActiveQuests.Remove(QuestID);

		// 완료된 퀘스트 목록에 추가
		CompletedQuests.Add(QuestID);

		// UI 반영 브로드캐스트
		OnQuestTurnedInSig.Broadcast(QuestID);
	}
}

const EQuestStatus UQuestComponent::GetQuestStatusFromQuestID(const FName QuestID) const
{
	if (const FActiveQuest* QuestData = ActiveQuests.Find(QuestID))
	{
		return QuestData->Status;
	}

	return EQuestStatus();
}

void UQuestComponent::GrantReward(const FQuestData& Quest)
{
	// 퀘스트 완료 보상 지급
	// 재화(골드) 추가
	if (AActor* Owner = GetOwner())
	{
		if (UInventoryComponent* InventoryComp = Owner->FindComponentByClass<UInventoryComponent>())
		{
			InventoryComp->AddGolds(Quest.RewardGold);
		}
	}
}