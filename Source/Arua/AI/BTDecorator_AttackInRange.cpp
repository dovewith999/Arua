// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_AttackInRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/ARAI.h"
#include "Enemy/Boss/ARBoss.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
	// 노드 이름 설정.
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool Result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// AIController가 제어하는 폰 정보.
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn)
	{
		return false;
	}


	AARBoss* AIPawn = Cast<AARBoss>(ControllingPawn);
	if (!AIPawn)
	{
		return false;
	}

	// 감지된 폰.
	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYERACTOR);
	if (!TargetObject)
	{
		return false;
	}

	// 감지된 Object 타입을 폰으로 변환.
	APawn* Target = Cast<APawn>(TargetObject);
	if (!Target)
	{
		return false;
	}

	// 감지된 캐릭터와의 거리 확인.
	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);

	// 공격 가능 범위 값 얻어오기.
	float AttackRange = AIPawn->GetBossAttackRange();

	// 캐릭터와의 거리가 공격 가능 범위 안에 있는지 확인 후 결과 반환.
	Result = (DistanceToTarget <= (AttackRange * 1.1));


	//근거리 공격과 원거리 공격 중 랜덤하게 선택하여 공격
	if (Result == true)
	{
		int32 RandomNum = FMath::RandRange(1, 1);//테스트를 위해 1로 설정

		if (RandomNum == 0)
		{
			UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
			if (BlackboardComp)
			{
				FName Type = FName("AttackNear");
				BlackboardComp->SetValueAsName(FName("PatternType"), Type);
			}
		}
		else
		{
			UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
			if (BlackboardComp)
			{
				FName Type = FName("AttackFar");
				BlackboardComp->SetValueAsName(FName("PatternType"), Type);
			}
		}
	}



	return Result;
}
