// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_SensePlayer.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "AI/ARAI.h"
#include "Controller/AruaPlayerController.h"
#include "Enemy/ARMonsterBase.h"
#include "Enemy/Boss/ARBoss.h"
#include "AbilitySystemComponent.h"

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


	AAruaPlayerController* PC = Cast<AAruaPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	APawn* PlayerPawn = PC->GetPawn();
	//Player 없는경우
	if (!PlayerPawn)
	{
		AARMonsterBase* OwnerMonster = Cast<AARMonsterBase>(AIPawn);
		OwnerMonster->IsSensed(false);
		//BB->SetValueAsBool(BBKEY_ISSENSED, false);
		return;
	}

	const FVector BossLocation = AIPawn->GetActorLocation();
	const FVector PlayerLocation = PlayerPawn->GetActorLocation();
	AARBoss* BossPawn = Cast<AARBoss>(AIPawn);
	AARMonsterBase* OwnerMonster = Cast<AARMonsterBase>(AIPawn);

	//boss 와 플레이어 사이 거리
	const float DistSq = FVector::DistSquared(BossLocation, PlayerLocation);
	float SenseRange = BossPawn->GetBossSenseRange();

	//감지 범위
	const float SenseRangeSq = SenseRange * SenseRange;


	// 둥지 벗어나지 않도록 하는 로직

	float NestRange = BossPawn->GetBossNestRange();
	FVector NestLocation = BB->GetValueAsVector(BBKEY_NESTPOS);

	const float DistNestSq = FVector::DistSquared(PlayerLocation, NestLocation);
	const float NestRangeSq = NestRange * NestRange;

	bool bIsInNest = false;

	if (DistNestSq <= NestRangeSq)
	{
		bIsInNest = true;
	}
	else
	{
		bIsInNest = false;
	}

	OwnerMonster->IsInNest(bIsInNest);

	// 플레이어와의 거리 감지해서 쫒아가는 로직

	bool bInSenseRange = false;

	if (DistSq <= SenseRangeSq)
	{
		bInSenseRange = true;
		PC->SetTargetBoss(BossPawn);
	}

	else
	{
		bInSenseRange = false;
		OwnerMonster->Recovery();
	}

	OwnerMonster->IsSensed(bInSenseRange);
	//BB->SetValueAsBool(BBKEY_ISSENSED, bInSenseRange);

	//디버그로 감지 범위
	DrawDebugSphere(
		AIPawn->GetWorld(),
		BossLocation,
		SenseRange,
		32,
		bInSenseRange ? FColor::Blue : FColor::Red,
		false,
		Interval,  // 다음 Tick까지 유지
		0,
		2.f
	);
}