// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Quest/QuestHUDWidget.h"
#include "UI/Quest/QuestHUDItemWidget.h"

#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/Quest/QuestComponent.h"

void UQuestHUDWidget::BindToQuestComponent(class UQuestComponent* InQuestComponent)
{
	if (!InQuestComponent) return;

	QuestComponent = InQuestComponent;

	// 퀘스트 컴포넌트의 이벤트 델리게이트 바인딩
	InQuestComponent->OnQuestAcceptedSig.AddDynamic(this, &UQuestHUDWidget::HandleQuestAccepted);
	InQuestComponent->OnQuestProgressSig.AddDynamic(this, &UQuestHUDWidget::HandleQuestProgress);
	InQuestComponent->OnQuestReadyToTurnInSig.AddDynamic(this, &UQuestHUDWidget::HandleQuestReadyToTurnIn);
	InQuestComponent->OnQuestTurnedInSig.AddDynamic(this, &UQuestHUDWidget::HandleQuestTurnedIn);

	// HUD 초기화
	ActiveQuestList->ClearChildren();
	QuestHUDItemMap.Empty();

	// 퀘스트 순번
	int32 QuestOrder = 1;

	for (const TPair<FName, FActiveQuest>& Pair : InQuestComponent->ActiveQuests)
	{
		const FActiveQuest& ActiveQuest = Pair.Value;

		// 퀘스트 항목 생성
		UQuestHUDItemWidget* QuestHUDItem = CreateQuestHUDItem(ActiveQuest.QuestData, QuestOrder++);
		if (!QuestHUDItem) continue;

		// 퀘스트 항목 진행도 초기화
		QuestHUDItem->UpdateProgress(ActiveQuest.CurrentCount, ActiveQuest.QuestData.ObjectiveCount);

		// 해당 퀘스트 항목이 목표 달성(완료 가능) 상태라면 그에 맞는 디자인 설정
		if (ActiveQuest.Status == EQuestStatus::ReadyToTurnIn)
		{
			QuestHUDItem->TurnInQuest();
		}
	}

	// 퀘스트 순번 재정렬
	ReIndexQuestOrders();
}

void UQuestHUDWidget::HandleQuestAccepted(const FQuestData& QuestData)
{
	// 새 퀘스트 항목 위젯 생성 후, 퀘스트 항목에 추가
	CreateQuestHUDItem(QuestData, ActiveQuestList->GetChildrenCount() + 1);
	
	// 퀘스트 순번 재정렬
	ReIndexQuestOrders();
}

void UQuestHUDWidget::HandleQuestProgress(FName QuestID, int32 CurrentCount)
{
	if (!QuestComponent) return;

	// 갱신할 퀘스트 항목 가져오기
	if (UQuestHUDItemWidget* QuestHUDItem = QuestHUDItemMap.FindRef(QuestID))
	{
		// 새 목표 수치로 갱신
		const int32 ObjectiveCount = QuestComponent->ActiveQuests[QuestID].QuestData.ObjectiveCount;
		QuestHUDItem->UpdateProgress(CurrentCount, ObjectiveCount);
	}
}

void UQuestHUDWidget::HandleQuestReadyToTurnIn(FName QuestID)
{
	if (!ActiveQuestList) return;

	if (UQuestHUDItemWidget* QuestHUDItem = QuestHUDItemMap.FindRef(QuestID))
	{
		// 목표 달성 시 라벨/아이콘 바꾸기
		QuestHUDItem->TurnInQuest();
	}
}

void UQuestHUDWidget::HandleQuestTurnedIn(FName QuestID)
{
	// 퀘스트 NPC에게 반납 완료 후 HUD 및 퀘스트 항목 맵에서 제거
	if (UQuestHUDItemWidget** FoundQuestHUDItem = QuestHUDItemMap.Find(QuestID))
	{
		if (UQuestHUDItemWidget* QuestHUDItem = *FoundQuestHUDItem)
		{
			// 퀘스트 항목에서 해당 위젯 제거
			if (ActiveQuestList && ActiveQuestList->HasChild(QuestHUDItem))
			{
				ActiveQuestList->RemoveChild(QuestHUDItem);
			}

			// 위젯 메모리 정리 (GC에 맡길 수도 있지만 명시적으로 제거)
			QuestHUDItem->RemoveFromParent();
		}

		// 퀘스트 항목 맵에서 제거
		QuestHUDItemMap.Remove(QuestID);
	}

	// 퀘스트 순번 재정렬
	ReIndexQuestOrders();

	// Todo 김준형: [완료 퀘스트 탭 추가 확장 가능]
}

UQuestHUDItemWidget* UQuestHUDWidget::CreateQuestHUDItem(const FQuestData& Data, int32 DisplayOrder)
{
	if (!QuestHUDItemWidgetClass || !ActiveQuestList) return nullptr;

	// 퀘스트 항목 위젯 인스턴스 생성
	UQuestHUDItemWidget* QuestHUDItem = CreateWidget<UQuestHUDItemWidget>(GetOwningPlayer(), QuestHUDItemWidgetClass);
	if (!QuestHUDItem) return nullptr;

	// 퀘스트 데이터 및 순번 초기화
	QuestHUDItem->InitializeQuest(Data, DisplayOrder);

	// VerticalBox에 추가
	ActiveQuestList->AddChildToVerticalBox(QuestHUDItem);

	// 퀘스트 항목 맵에 추가
	QuestHUDItemMap.Add(Data.QuestID, QuestHUDItem);

	return QuestHUDItem;
}

void UQuestHUDWidget::ReIndexQuestOrders()
{
	if (!ActiveQuestList) return;

	const int32 Num = ActiveQuestList->GetChildrenCount();
	for (int32 i = 0; i < Num; ++i)
	{
		if (UQuestHUDItemWidget* QuestHUDItem = Cast<UQuestHUDItemWidget>(ActiveQuestList->GetChildAt(i)))
		{
			// 순서는 0부터 시작하므로, +1해서 순번 지정
			QuestHUDItem->UpdateOrderNumber(i + 1);
		}
	}
}
