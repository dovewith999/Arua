// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ARAI.h"
#include "Enemy/Boss/ARBoss.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
    NodeName = TEXT("TurnToTarget");
    bNotifyTick = true; //  Tick 활성화
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    return EBTNodeResult::InProgress; // 한 프레임에 끝내지 않고 진행 상태로 둠
}

void UBTTask_TurnToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYERACTOR));

    if (!ControllingPawn || !TargetPawn)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    AARBoss* AIPawn = Cast<AARBoss>(ControllingPawn);
    if (!AIPawn)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    FVector LookVector = TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();
    FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
    FRotator CurrentRot = ControllingPawn->GetActorRotation();

    FRotator FinalRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaSeconds, AIPawn->GetBossTurnSpeed());
    ControllingPawn->SetActorRotation(FinalRot);

    //  목표 방향과 거의 같아졌는지 확인 후 종료
    if (FMath::Abs(FinalRot.Yaw - TargetRot.Yaw) < 5.0f)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
