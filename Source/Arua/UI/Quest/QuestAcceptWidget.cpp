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
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, TEXT("HandleAccept"));
	
	// 위젯 숨기기
	this->SetVisibility(ESlateVisibility::Collapsed);

	// TODO 김준형: 퀘스트 컴포넌트 등록, 보상 처리 등 추가
}

void UQuestAcceptWidget::HandleClose()
{
	GEngine->AddOnScreenDebugMessage(1, 1, FColor::Blue, TEXT("HandleClose"));
	
	// 위젯 숨기기
	this->SetVisibility(ESlateVisibility::Collapsed);
}
