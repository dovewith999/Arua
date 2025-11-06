// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Quest/QuestAcceptWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UQuestAcceptWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!AcceptButton || !CloseButton) return;

	// 버튼 클릭 함수 바인딩, 수락/반납 모두 처리 (모드에 따라 분기)
	AcceptButton->OnClicked.AddDynamic(this, &UQuestAcceptWidget::HandleAcceptOrTurnIn);
	CloseButton->OnClicked.AddDynamic(this, &UQuestAcceptWidget::HandleClose);
}

void UQuestAcceptWidget::SetQuestInfo(const FQuestData& InQuestData, EQuestAcceptMode InMode)
{
	// 퀘스트 데이터 및 퀘스트 모드 초기화
	CurrentQuestData = InQuestData;
	CurrentMode = InMode;

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


	// 수락 버튼 라벨을 모드에 맞게 갱신
	if (AcceptButtonLabel)
	{
		AcceptButtonLabel->SetText(CurrentMode == EQuestAcceptMode::Accept
			? FText::FromString(TEXT("임무 수령"))
			: FText::FromString(TEXT("보상 수령"))
		);
	}
}

void UQuestAcceptWidget::HandleAcceptOrTurnIn()
{
	// 창을 닫는다(숨김), 외부에서 실제 처리(등록/보상) 수행
	SetVisibility(ESlateVisibility::Collapsed);

	// 퀘스트 수락 브로드캐스트(위젯 제거/퀘스트 등록)
	if (CurrentMode == EQuestAcceptMode::Accept)
	{
		OnQuestAccept.Broadcast(CurrentQuestData.QuestID);
	}
	// 퀘스트 보상 수령 브로드캐스트
	else
	{
		OnQuestTurnIn.Broadcast(CurrentQuestData.QuestID);
	}
}

void UQuestAcceptWidget::HandleClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
	OnQuestClose.Broadcast();
}
