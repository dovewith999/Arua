// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARAIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AI/ARAI.h"
#include "Kismet/GameplayStatics.h"

AARAIController::AARAIController()
{
	//사용할 BlackBoard
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef
	(TEXT("/Game/Personal/LEE_J_S/AI/BB_Boss_ElementalDragon.BB_Boss_ElementalDragon"));
	if (BBAssetRef.Succeeded())
	{
		BBAsset = BBAssetRef.Object;
	}

	//사용할 BehaviorTree
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef
	(TEXT("/Game/Personal/LEE_J_S/AI/BT_Boss_ElementalDragon.BT_Boss_ElementalDragon"));
	if (BTAssetRef.Succeeded())
	{
		BTAsset = BTAssetRef.Object;
	}


}

void AARAIController::RunAI()
{
	// 블랙보드 컴포넌트 받아오기.
	UBlackboardComponent* BB = Blackboard.Get();

	// 블랙보드 사용 설정.
	if (UseBlackboard(BBAsset, BB))
	{
		// 보스의 초기 위치 저장
		APawn* MyPawn = GetPawn();
		if (MyPawn)
		{
			Blackboard->SetValueAsVector(BBKEY_BOSSPOS, MyPawn->GetActorLocation());
		}

		// 플레이어 정보 저장
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (PlayerPawn)
		{
			Blackboard->SetValueAsObject(BBKEY_PLAYERACTOR, PlayerPawn);
			Blackboard->SetValueAsVector(BBKEY_PLAYERPOS, PlayerPawn->GetActorLocation());
		}


		// 행동트리 실행.
		bool Result = RunBehaviorTree(BTAsset);

		// 확인.
		ensureAlways(Result);
	}


}

void AARAIController::StopAI()
{
	// 실행 중인 BT 컴포넌트 받아오기.
	UBehaviorTreeComponent* BT
		= Cast<UBehaviorTreeComponent>(BrainComponent);

	if (BT)
	{
		BT->StopTree();
	}
}

void AARAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}
