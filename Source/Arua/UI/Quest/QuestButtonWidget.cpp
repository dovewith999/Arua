// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Quest/QuestButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UQuestButtonWidget::Init(const FQuestData& InQuestData, bool IsCompletedQuest)
{
	// 퀘스트 ID 초기화
	QuestID = InQuestData.QuestID;

	if (!QuestButton || !QuestNameText)
	{
		//GEngine->AddOnScreenDebugMessage(0, 10.f, FColor::Red, FString("InValid: QuestButton or QuestNameText"));
		return;
	}

	if (QuestAcceptedIcon)
	{
		QuestAcceptedIcon->SetVisibility(IsCompletedQuest ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}
	if (QuestTurnedInIcon)
	{
		QuestTurnedInIcon->SetVisibility(IsCompletedQuest ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	// 퀘스트 버튼 클릭 바인딩
	QuestButton->OnClicked.AddDynamic(this, &UQuestButtonWidget::HandleOnClicked);

	// 퀘스트 타이틀 텍스트 설정
	QuestNameText->SetText(InQuestData.Title);
}

void UQuestButtonWidget::HandleOnClicked()
{
	OnClicked.Broadcast(QuestID);
}