// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Quest/QuestSelectWidget.h"
#include "UI/Quest/QuestButtonWidget.h"
#include "Components/VerticalBox.h"

void UQuestSelectWidget::InitQuestList(const TArray<FName>& QuestIDs)
{
	if (!QuestButtonContainer || !QuestButtonWidgetClass) return;

	// 패널 위젯 초기화
	QuestButtonContainer->ClearChildren();

	// 역방향(위로)으로 퀘스트 버튼 위젯을 쌓기 위햇 역순으로 배열 순회
	for (int i = QuestIDs.Num() - 1; i >= 0; --i)
	{
		const FName QuestID = QuestIDs[i];

		if (QuestID.IsValid())
		{
			// 퀘스트 버튼 위젯 생성 및 퀘스트 ID 초기화
			UQuestButtonWidget* QuestButton = CreateWidget<UQuestButtonWidget>(GetOwningPlayer(), QuestButtonWidgetClass);
			if (!QuestButton) continue;

			QuestButton->Init(QuestID);

			// 퀘스트 버튼 클릭 델리게이트에 퀘스트 선택 함수 바인딩
			QuestButton->OnClicked.AddDynamic(this, &UQuestSelectWidget::HandleQuestButtonClicked);

			// 패널에 추가
			QuestButtonContainer->AddChildToVerticalBox(QuestButton);
		}
	}
}

void UQuestSelectWidget::HandleQuestButtonClicked(FName InQuestID)
{
	OnQuestSelected.Broadcast(InQuestID);
}
