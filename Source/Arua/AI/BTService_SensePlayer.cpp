// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_SensePlayer.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "AI/ARAI.h"
#include "Controller/AruaPlayerController.h"
#include "Enemy/ARMonsterBase.h"

UBTService_SensePlayer::UBTService_SensePlayer()
{
	NodeName = TEXT("SensePlayer");

	Interval = 0.1f;
}

void UBTService_SensePlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AIC = OwnerComp.GetAIOwner();
	APawn* AIPawn = AIC->GetPawn();


		AAruaPlayerController* PC = Cast<AAruaPlayerController>(UGameplayStatics::GetPlayerController(AIPawn, 0));
		APawn* PlayerPawn = PC->GetPawn();
		//Player 없는경우
		if (!PlayerPawn)
		{
			BB->SetValueAsBool(BBKEY_ISSENSED, false);
			return;

		}

		const FVector BossLocation = AIPawn->GetActorLocation();
		const FVector PlayerLocation = PlayerPawn->GetActorLocation();

		//boss 와 플레이어 사이 거리
		const float DistSq = FVector::DistSquared(BossLocation, PlayerLocation);
		
		//감지 범위
		const float SenseRadiusSq = SenseRadius * SenseRadius;


		
		bool bInSenseRange = false;

		if (DistSq <= SenseRadiusSq)
			bInSenseRange = true;
		else
			bInSenseRange = false;

		BB->SetValueAsBool(BBKEY_ISSENSED, bInSenseRange);

		//디버그로 감지 범위
		DrawDebugSphere(
			AIPawn->GetWorld(),
			BossLocation,
			SenseRadius,
			32,
			bInSenseRange ? FColor::Blue : FColor::Red,
			false,
			Interval,  // 다음 Tick까지 유지
			0,
			2.f
		);


}