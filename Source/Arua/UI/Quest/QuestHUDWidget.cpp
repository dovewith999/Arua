// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Quest/QuestHUDWidget.h"

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
	QuestRows.Empty();

	// 이미 진행중인 퀘스트 표시 (게임 중간에 재바인딩되는 경우 대비)
	for (const TPair<FName, FActiveQuest>& Pair : InQuestComponent->ActiveQuests)
	{
		const FActiveQuest& QuestEntry = Pair.Value;

		HandleQuestAccepted(QuestEntry.QuestData);
		HandleQuestProgress(QuestEntry.QuestData.QuestID, QuestEntry.CurrentCount);

		// 이미 반납 대기 상태라면 즉시 스타일 업데이트
		if (QuestEntry.Status == EQuestStatus::ReadyToTurnIn)
		{
			HandleQuestReadyToTurnIn(QuestEntry.QuestData.QuestID);
		}
	}
}

void UQuestHUDWidget::HandleQuestAccepted(const FQuestData& QuestData)
{
	if (!ActiveQuestList || QuestRows.Contains(QuestData.QuestID)) return;

	// #1: 새 Row(HorizontalBox) 생성 (퀘스트 제목 + 진행도)
	UHorizontalBox* QuestRow = NewObject<UHorizontalBox>(this);

	// #2: 퀘스트 제목 설정
	UTextBlock* QuestTitleText = NewObject<UTextBlock>(QuestRow);
	QuestTitleText->SetText(QuestData.Title);
	QuestTitleText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	QuestRow->AddChildToHorizontalBox(QuestTitleText);

	// #3: 진행도 표시 (초기값 0 / 목표수)
	UTextBlock* QuestProgressText = NewObject<UTextBlock>(QuestRow);
	QuestProgressText->SetText(FText::FromString(FString::Printf(TEXT(" 0/%d"), QuestData.ObjectiveCount)));
	QuestProgressText->SetColorAndOpacity(FSlateColor(FLinearColor::Gray));
	QuestRow->AddChildToHorizontalBox(QuestProgressText);

	// #4: VerticalBox에 Row 추가
	ActiveQuestList->AddChildToVerticalBox(QuestRow);

	// #5: QuestID → Row 매핑 저장
	QuestRows.Add(QuestData.QuestID, QuestRow);
}

void UQuestHUDWidget::HandleQuestProgress(FName QuestID, int32 CurrentCount)
{
	if (!QuestComponent) return;

	// 해당 퀘스트 Row 찾기
	if (UHorizontalBox* QuestRow = QuestRows.FindRef(QuestID))
	{
		if (QuestRow->GetChildrenCount() >= 2)
		{
			// 두 번째 자식이 진행도 텍스트
			if (UTextBlock* ProgressText = Cast<UTextBlock>(QuestRow->GetChildAt(1)))
			{
				const int32 TargetCount = QuestComponent->ActiveQuests[QuestID].QuestData.ObjectiveCount;
				ProgressText->SetText(
					FText::FromString(FString::Printf(TEXT(" %d/%d"), CurrentCount, TargetCount))
				);
			}
		}
	}
}

void UQuestHUDWidget::HandleQuestReadyToTurnIn(FName QuestID)
{
	if (!ActiveQuestList) return;

	// 목표 달성 후 완료 대기 상태로 전환
	if (UHorizontalBox* QuestRow = QuestRows.FindRef(QuestID))
	{
		if (QuestRow->GetChildrenCount() >= 2)
		{
			if (UTextBlock* Title = Cast<UTextBlock>(QuestRow->GetChildAt(0)))
			{
				FString Label = FString::Printf(TEXT("%s [완료 가능]"), *Title->GetText().ToString());
				Title->SetText(FText::FromString(Label));
				Title->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 0.85f, 0.2f)));
			}

			if (UTextBlock* Progress = Cast<UTextBlock>(QuestRow->GetChildAt(1)))
			{
				Progress->SetColorAndOpacity(FSlateColor(FLinearColor(0.2f, 1.f, 0.2f)));
			}
		}
	}
}

void UQuestHUDWidget::HandleQuestTurnedIn(FName QuestID)
{
	// 퀘스트 NPC에게 반납 완료 후 HUD에서 제거
	if (UHorizontalBox* Row = QuestRows.FindRef(QuestID))
	{
		ActiveQuestList->RemoveChild(Row);
		QuestRows.Remove(QuestID);
	}

	// Todo 김준형: [완료 퀘스트 탭 추가 확장 가능]
}
