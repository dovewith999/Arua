// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Quest/QuestAcceptWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UQuestAcceptWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!AcceptButton || !CloseButton) return;

	// 버튼 클릭 함수 바인딩
	AcceptButton->OnClicked.AddDynamic(this, &UQuestAcceptWidget::HandleAccept);
	CloseButton->OnClicked.AddDynamic(this, &UQuestAcceptWidget::HandleClose);
}

void UQuestAcceptWidget::SetQuestInfo(const FQuestData& InQuestData)
{
	CurrentQuestData = InQuestData;

	if (QuestTitleText)
	{
		QuestTitleText->SetText(InQuestData.Title);
	}
	if (QuestObjectiveText)
	{
		QuestObjectiveText->SetText(InQuestData.Objective);
	}
	if (QuestDescriptionText)
	{
		QuestDescriptionText->SetText(InQuestData.Description);
	}
	if (QuestRewardText)
	{
		FString RewardStr = FString::Printf(TEXT("골드 %d개"), InQuestData.RewardGold);
		QuestRewardText->SetText(FText::FromString(RewardStr));
	}
}

void UQuestAcceptWidget::HandleAccept()
{
	// 위젯 숨기기
	this->SetVisibility(ESlateVisibility::Collapsed);

	// 퀘스트 수락 브로드캐스트(위젯 제거/퀘스트 등록을 위해)
	OnQuestAccept.Broadcast(CurrentQuestData.QuestID);
}

void UQuestAcceptWidget::HandleClose()
{
	// 위젯 숨기기
	this->SetVisibility(ESlateVisibility::Collapsed);
}
