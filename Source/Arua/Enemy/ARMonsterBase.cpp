// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/ARMonsterBase.h"
#include "UI/WidgetComponent/ARWidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet/ARAttributeSetBase.h"
#include "AttributeSet/MonsterAttributeSet.h"
#include "UI/ViewModel/BossViewModel.h"
#include "UI/Model/BossData.h"
#include "AI/ARAIController.h"
#include "Tag/AruaGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AI/ARAI.h"
#include "Behaviortree/BlackboardComponent.h"
#include "Actors/Items/ItemPickupActor.h"

AARMonsterBase::AARMonsterBase()
{
	// Widget Component
	TargetLock = CreateDefaultSubobject<UARWidgetComponent>(TEXT("Widget"));
	TargetLock->SetupAttachment(GetMesh());

	// 위치 조정
	float CenterPosition = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	TargetLock->SetRelativeLocation(FVector(0.f, 0.f, CenterPosition));

	// 어떤 Widget BP를 사용해 그릴지
	static ConstructorHelpers::FClassFinder<UUserWidget> TargetLockViewRef(TEXT("/Game/Personal/LIM_H_S/UI/WBP_TargetLock.WBP_TargetLock_C"));
	if (TargetLockViewRef.Succeeded())
	{
		// 컴포넌트에서 사용할 위젯 클래스 설정
		TargetLock->SetWidgetClass(TargetLockViewRef.Class);

		// 위젯을 그릴 공간 지정
		TargetLock->SetWidgetSpace(EWidgetSpace::Screen);

		// 그릴 크기 지정
		TargetLock->SetDrawSize(FVector2D(100.f, 100.f));
		TargetLock->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		TargetLock->bHiddenInGame = true;
	}
}

void AARMonsterBase::SetTargetLockWidget(bool InShow)
{
	TargetLock->bHiddenInGame = InShow;
}

void AARMonsterBase::IsSensed(bool InIsSensed)
{
	if (AARAIController* AIController = Cast<AARAIController>(GetController()))
	{
		if (UBlackboardComponent* BB = AIController->GetBlackboardComponent())
		{
			BB->SetValueAsBool(BBKEY_ISSENSED, InIsSensed);
		}
	}
}

void AARMonsterBase::IsInNest(bool InIsInNest)
{
	if (AARAIController* AIController = Cast<AARAIController>(GetController()))
	{
		if (UBlackboardComponent* BB = AIController->GetBlackboardComponent())
		{
			BB->SetValueAsBool(BBKEY_ISINNEST, InIsInNest);
		}
	}
}

void AARMonsterBase::Recovery()
{
	if (ASC == nullptr)
	{
		return;
	}

	/*UARAttributeSetBase**/ AttributeSet = const_cast<UARAttributeSetBase*>(Cast<UARAttributeSetBase>(ASC->GetAttributeSet(UARAttributeSetBase::StaticClass())));
	
	if (AttributeSet)
	{
		float MaxHealth = AttributeSet->GetMaxHealth();
		AttributeSet->SetHealth(MaxHealth);
	}
}


void AARMonsterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AARMonsterBase::SetDead()
{
	Super::SetDead();

	if (AARAIController* AIController = Cast<AARAIController>(GetController()))
	{
		AIController->StopAI();

	}

	// 몬스터 사망 시, 드롭 아이템 랜덤 드롭하기
	if (!DropItems.IsEmpty())
	{
		// 드롭 아이템 목록에서 드롭할 랜덤 인덱스 구하기
		const int32 RandomIndex = FMath::RandRange(0, DropItems.Num() - 1);
		TSubclassOf<AItemPickupActor> ItemClass = DropItems[RandomIndex];
		if (!ItemClass || !GetWorld()) return;

		// 아이템을 드롭할 위치 구하기
		const FVector ActorLocation = GetActorLocation();
		const FVector TraceStart = ActorLocation + FVector(0.f, 0.f, 50.f);
		const FVector TraceEnd = ActorLocation - FVector(0.f, 0.f, 2000.f);

		FHitResult HitResult;
		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(DropTrace), false, this);

		FVector SpawnLocation = ActorLocation;

		if (GetWorld()->LineTraceSingleByChannel(
			HitResult,
			TraceStart,
			TraceEnd,
			ECC_Visibility,
			QueryParams))
		{
			// 지면에 맞춰 약간 위로 띄워서 스폰
			SpawnLocation = HitResult.Location + FVector(0.f, 0.f, 5.f);
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		// 아이템 스폰
		GetWorld()->SpawnActor<AItemPickupActor>(
			ItemClass,
			SpawnLocation,
			FRotator::ZeroRotator,
			SpawnParams
		);
	}
}

void AARMonsterBase::OnHitByAttack_Implementation(const FHitResult& HitResult, AActor* InInstigator)
{
	if (InInstigator)
	{
		// GameplayCue 파라미터 설정
		FGameplayCueParameters CueParams;
		CueParams.Instigator = InInstigator;
		CueParams.EffectCauser = InInstigator;
		CueParams.Location = HitResult.ImpactPoint;
		CueParams.Normal = HitResult.ImpactNormal;
		CueParams.SourceObject = this;

		if (ASC)
		{
			// 몬스터 Hit Cue 실행
			ASC->ExecuteGameplayCue(
				AruaGamePlayTags::GameplayCue_Monster_Hit,
				CueParams
			);
		}
	}
}
