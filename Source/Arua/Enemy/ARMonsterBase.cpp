// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/ARMonsterBase.h"
#include "UI/WidgetComponent/ARWidgetComponent.h"

AARMonsterBase::AARMonsterBase()
{
	// Widget Component
	TargetLock = CreateDefaultSubobject<UARWidgetComponent>(TEXT("Widget"));
	TargetLock->SetupAttachment(GetMesh());

	// 위치 조정
	TargetLock->SetRelativeLocation(FVector(0.f, 0.f, 200.f));

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
	}
}

void AARMonsterBase::BeginPlay()
{
	Super::BeginPlay();
}
