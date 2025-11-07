// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Quest/QuestHUDItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Styling/SlateColor.h"

void UQuestHUDItemWidget::InitializeQuest(const FQuestData& InQuestData, int32 DisplayOrder)
{
	// 퀘스트 ID 초기화
	QuestID = InQuestData.QuestID;

	if (QuestOrderText)
	{
		QuestOrderText->SetText(FText::AsNumber(DisplayOrder));
	}
	if (QuestTitleText)
	{
		QuestTitleText->SetText(InQuestData.Title);
	}
	if (QuestObjectiveText)
	{
		QuestObjectiveText->SetText(InQuestData.Objective);
	}
	if (QuestAcceptedIcon)
	{
		QuestAcceptedIcon->SetVisibility(ESlateVisibility::Visible);
	}
	if (QuestTurnedInIcon)
	{
		QuestTurnedInIcon->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 진행도 초기화
	UpdateProgress(0, InQuestData.ObjectiveCount);
}

void UQuestHUDItemWidget::UpdateProgress(int32 Current, int32 Target)
{
	// 진행도 갱신
	if (QuestProgressText)
	{
		// 목표 수치가 있는 퀘스트인 경우
		if (Target > 0)
		{
			const FString Str = FString::Printf(TEXT("%d/%d"), Current, Target);
			QuestProgressText->SetText(FText::FromString(Str));
		}

		// 대화같은 목표 수치가 없는 경우는 예외 처리
	}
}

void UQuestHUDItemWidget::UpdateOrderNumber(int32 NewOrder)
{
	if (QuestOrderText)
	{
		QuestOrderText->SetText(FText::AsNumber(NewOrder));
	}
}

void UQuestHUDItemWidget::TurnInQuest()
{
	// 퀘스트 타이틀/제목 텍스트에 [임무 완료] 추가 및 색상 변경
	if (QuestTitleText)
	{
		QuestTitleText->SetText(FText::FromString(QuestTitleText->GetText().ToString() + TEXT("  [완료]")));
		QuestTitleText->SetColorAndOpacity(QuestTitleTextTurnInColor);
	}

	// 진행도 텍스트에 [임무 완료] 추가 및 색상 변경
	if (QuestProgressText)
	{
		QuestProgressText->SetColorAndOpacity(QuestProgressTextTurnInColor);
	}

	// 수락/완료 퀘스트 아이콘 가시화 전환
	if (QuestAcceptedIcon)
	{
		QuestAcceptedIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (QuestTurnedInIcon)
	{
		QuestTurnedInIcon->SetVisibility(ESlateVisibility::Visible);
	}
}
