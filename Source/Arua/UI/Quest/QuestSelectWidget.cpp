// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Quest/QuestSelectWidget.h"
#include "UI/Quest/QuestButtonWidget.h"
#include "UI/Quest/QuestAcceptWidget.h"

#include "Components/VerticalBox.h"
#include "Components/SizeBox.h"
#include "Components/Quest/QuestComponent.h"
#include "Engine/DataTable.h"
#include "DataTables/QuestData.h"

static const FString QuestSelectWidgetContext(TEXT("Quest Data Context"));

void UQuestSelectWidget::InitQuestList(APawn* InInteractor, const TArray<FName>& QuestIDs, class UDataTable* InQuestDataTable)
{
	if (!QuestButtonContainer || !QuestButtonWidgetClass || !InQuestDataTable || !InInteractor) return;

	// 위젯 데이터 초기화
	QuestDataTable = InQuestDataTable;
	QuestButtonContainer->ClearChildren();
	AcceptButtons.Empty();
	TurnInButtons.Empty();

	// 플레이어의 퀘스트 컴포넌트 저장
	UQuestComponent* QuestComp = InInteractor->FindComponentByClass<UQuestComponent>();
	if (!QuestComp) return;

	// #1: 수락 가능한 퀘스트 버튼 생성
	// 역방향(위로)으로 퀘스트 버튼 위젯을 쌓기 위햇 역순으로 배열 순회
	for (int i = QuestIDs.Num() - 1; i >= 0; --i)
	{
		// 퀘스트 ID에 맞는 퀘스트 데이터 테이블 가져오기
		FQuestData* QuestData = QuestDataTable->FindRow<FQuestData>(QuestIDs[i], QuestSelectWidgetContext);
		if (!QuestData) continue;

		// 해당 퀘스트가 수락하지 않은 상태인지 확인
		if (QuestComp->GetQuestStatusFromQuestID(QuestIDs[i]) != EQuestStatus::None) continue;

		// 퀘스트 버튼 위젯 생성
		UQuestButtonWidget* QuestButton = CreateWidget<UQuestButtonWidget>(GetOwningPlayer(), QuestButtonWidgetClass);
		if (!QuestButton) continue;

		// 퀘스트 버튼 클릭 델리게이트에 퀘스트 버튼 위젯 선택(클릭) 콜백 함수 바인딩
		QuestButton->Init(*QuestData, false);
		QuestButton->OnClicked.AddDynamic(this, &UQuestSelectWidget::HandleQuestButtonClicked);

		// 패널에 추가
		QuestButtonContainer->AddChildToVerticalBox(QuestButton);

		// 선택 가능한 퀘스트 버튼(퀘스트 목록)에 추가
		AcceptButtons.Add(QuestData->QuestID, QuestButton);
	}

	// #2: 보상 수령 가능한 퀘스트 버튼 생성
	// 현재 진행중인 퀘스트 리스트를 순회하며 보상 수령 가능한 퀘스트 검색
	for (const TPair<FName, FActiveQuest>& Pair : QuestComp->ActiveQuests)
	{
		const FActiveQuest& Quest = Pair.Value;
		if (Quest.Status != EQuestStatus::ReadyToTurnIn) continue;

		// 이 NPC가 처리할 수 있는 퀘스트인지 필터링
		if (!QuestIDs.Contains(Quest.QuestData.QuestID)) continue;

		UQuestButtonWidget* Button = CreateWidget<UQuestButtonWidget>(GetOwningPlayer(), QuestButtonWidgetClass);
		if (!Button) continue;

		// 버튼 표시 텍스트 앞에 [완료] 추가
		FQuestData Labeled = Quest.QuestData;
		Labeled.Title = FText::FromString(TEXT("[완료] ") + Quest.QuestData.Title.ToString());
		Button->Init(Labeled, true);

		// 퀘스트 버튼 클릭 함수 바인딩
		Button->OnClicked.AddDynamic(this, &UQuestSelectWidget::HandleQuestButtonClicked);

		// 패널에 추가
		QuestButtonContainer->AddChildToVerticalBox(Button);
		TurnInButtons.Add(Quest.QuestData.QuestID, Button);
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

		// 퀘스트 수락/보상 수령 함수 바인딩
		QuestAcceptWidgetInstance->OnQuestAccept.AddDynamic(this, &UQuestSelectWidget::HandleOnQuestAccepted);
		QuestAcceptWidgetInstance->OnQuestTurnIn.AddDynamic(this, &UQuestSelectWidget::HandleOnQuestTurnIn);
	}

	// 해당 퀘스트 창이 어떤 모드인지 확인
	const bool bIsTurnIn = TurnInButtons.Contains(InQuestID);
	const EQuestAcceptMode Mode = bIsTurnIn ? EQuestAcceptMode::TurnIn : EQuestAcceptMode::Accept;

	// 어떤 모드로 열지 결정 (반납 버튼 목록에 있으면 TurnIn, 아니면 Accept)
	QuestAcceptWidgetInstance->SetQuestInfo(*QuestData, Mode);
	QuestAcceptWidgetInstance->SetVisibility(ESlateVisibility::Visible);
}

void UQuestSelectWidget::HandleOnQuestAccepted(FName QuestID)
{
	// 수락 버튼 제거 (NPC/위젯에서 목록 정리)
	if (UQuestButtonWidget** Found = AcceptButtons.Find(QuestID))
	{
		if (UQuestButtonWidget* Button = *Found)
		{
			QuestButtonContainer->RemoveChild(Button);
		}

		AcceptButtons.Remove(QuestID);
	}

	// 외부(퀘스트 액터)에게 “수락됨” 브로드캐스트
	OnQuestAccepted.Broadcast(QuestID);

	// 수락창 닫기(숨김)
	if (QuestAcceptWidgetInstance)
	{
		QuestAcceptWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UQuestSelectWidget::HandleOnQuestTurnIn(FName QuestID)
{
	// 반납 버튼 제거
	if (UQuestButtonWidget** Found = TurnInButtons.Find(QuestID))
	{
		if (UQuestButtonWidget* Button = *Found)
		{
			QuestButtonContainer->RemoveChild(Button);
		}
		TurnInButtons.Remove(QuestID);
	}

	// 외부(퀘스트 액터)에게 “반납 요청” 브로드캐스트
	OnQuestTurnInRequested.Broadcast(QuestID);

	// 수락창 닫기
	if (QuestAcceptWidgetInstance)
	{
		QuestAcceptWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
	}
}
