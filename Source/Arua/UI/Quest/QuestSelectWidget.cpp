// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Quest/QuestSelectWidget.h"
#include "UI/Quest/QuestButtonWidget.h"
#include "UI/Quest/QuestAcceptWidget.h"

#include "Components/VerticalBox.h"
#include "Components/SizeBox.h"
#include "Engine/DataTable.h"
#include "DataTables/QuestData.h"

static const FString QuestSelectWidgetContext(TEXT("Quest Data Context"));

void UQuestSelectWidget::InitQuestList(const TArray<FName>& QuestIDs, class UDataTable* InQuestDataTable)
{
	if (!QuestButtonContainer || !QuestButtonWidgetClass || !InQuestDataTable) return;

	// 퀘스트 데이터 테이블 저장
	QuestDataTable = InQuestDataTable;

	// 역방향(위로)으로 퀘스트 버튼 위젯을 쌓기 위햇 역순으로 배열 순회
	for (int i = QuestIDs.Num() - 1; i >= 0; --i)
	{
		// 퀘스트 ID에 맞는 퀘스트 데이터 테이블 가져오기
		FQuestData* QuestData = QuestDataTable->FindRow<FQuestData>(QuestIDs[i], QuestSelectWidgetContext);
		if (!QuestData) continue;

		// 퀘스트 버튼 위젯 생성
		UQuestButtonWidget* QuestButton = CreateWidget<UQuestButtonWidget>(GetOwningPlayer(), QuestButtonWidgetClass);
		if (!QuestButton) continue;

		// 퀘스트 버튼 클릭 델리게이트에 퀘스트 버튼 위젯 선택(클릭) 콜백 함수 바인딩
		QuestButton->Init(*QuestData);
		QuestButton->OnClicked.AddDynamic(this, &UQuestSelectWidget::HandleQuestButtonClicked);

		// 패널에 추가
		QuestButtonContainer->AddChildToVerticalBox(QuestButton);

		// 선택 가능한 퀘스트 버튼(퀘스트 목록)에 추가
		ButtonMap.Add(QuestData->QuestID, QuestButton);
	}
}

void UQuestSelectWidget::HandleQuestButtonClicked(FName InQuestID)
{
	if (!QuestDataTable || !QuestAcceptWidgetClass || !QuestAcceptContainer) return;

	// 퀘스트 ID를 통해 해당 퀘스트 데이터 가져오기
	FQuestData* QuestData = QuestDataTable->FindRow<FQuestData>(InQuestID, QuestSelectWidgetContext);
	if (!QuestData) return;

	// 퀘스트 수락 위젯 생성
	if (!QuestAcceptWidgetInstance)
	{
		QuestAcceptWidgetInstance = CreateWidget<UQuestAcceptWidget>(GetOwningPlayer(), QuestAcceptWidgetClass);
		if (!QuestAcceptWidgetInstance) return;

		// 컨테이너에 위젯 추가
		QuestAcceptContainer->AddChild(QuestAcceptWidgetInstance);
	}

	// 퀘스트 수락 위젯 초기화 및 가시화
	QuestAcceptWidgetInstance->SetQuestInfo(*QuestData);
	QuestAcceptWidgetInstance->SetVisibility(ESlateVisibility::Visible);

	// 수락창 자체에서 퀘스트 위젯을 관리하지만, 수락한 퀘스트의 목록 제거는 여기서 처리
	QuestAcceptWidgetInstance->OnQuestAccept.AddDynamic(this, &UQuestSelectWidget::HandleOnQuestAccepted);
}

void UQuestSelectWidget::HandleOnQuestAccepted(FName QuestID)
{
	// 버튼 제거
	if (UQuestButtonWidget** Found = ButtonMap.Find(QuestID))
	{
		if (UQuestButtonWidget* Btn = *Found)
		{
			QuestButtonContainer->RemoveChild(Btn);
			ButtonMap.Remove(QuestID);
		}
	}

	// 퀘스트 수락 브로드캐스트
	OnQuestAccepted.Broadcast(QuestID);
}
