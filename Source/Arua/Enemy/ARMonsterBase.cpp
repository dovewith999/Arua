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

void AARMonsterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AARMonsterBase::SetDead()
{
	Super::SetDead();

	if(AARAIController* AIController = Cast<AARAIController>(GetController()))
	{
		AIController->StopAI();
	}
}

void AARMonsterBase::OnHitByAttack_Implementation(const FHitResult& HitResult, AActor* InInstigator)
{
	ACharacter* TargetCharacter = Cast<ACharacter>(HitResult.GetActor());

	if (TargetCharacter && InInstigator)
	{
		// GameplayCue 파라미터 설정
		FGameplayCueParameters CueParams;
		CueParams.Instigator = InInstigator;
		CueParams.EffectCauser = InInstigator;
		CueParams.Location = HitResult.ImpactPoint;
		CueParams.Normal = HitResult.ImpactNormal;
		CueParams.SourceObject = HitResult.GetActor();

		// 몬스터 ASC 가져오기
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetCharacter);

		if (TargetASC)
		{
			// 몬스터 Hit Cue 실행
			TargetASC->ExecuteGameplayCue(
				AruaGamePlayTags::GameplayCue_Monster_Hit,
				CueParams
			);
		}
	}
}
