// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GC/ARGC_SpecialAttack.h"
#include "Kismet/GameplayStatics.h"

UARGC_SpecialAttack::UARGC_SpecialAttack()
{
}

bool UARGC_SpecialAttack::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	Super::OnExecute_Implementation(Target, Parameters);

	//StartOffsetEffect();

	return true;
}

void UARGC_SpecialAttack::StartOffsetEffect()
{
	if (!UVOffsetMID)
	{
		// Post Process Volume 찾기(레벨 탐색)
		APostProcessVolume* PPVolume = nullptr;
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APostProcessVolume::StaticClass(), FoundActors);

		if (FoundActors.Num() > 0)
		{
			PPVolume = Cast<APostProcessVolume>(FoundActors[0]);
		}

		if (!PPVolume)
		{
			return;
		}

		// 머티리얼 인스턴스 찾기 및 MID 생성 (M_UVOffset_Inst를 가정)
		// Post Process Volume의 Settings Array에서 머티리얼을 찾아야 함
		FWeightedBlendables Blendables = PPVolume->Settings.WeightedBlendables;

		if (Blendables.Array.Num() > 0)
		{
			UMaterialInterface* PostProcessMat = Cast<UMaterialInterface>(Blendables.Array[0].Object);

			// UMaterialInstanceDynamic으로 변환
			if (PostProcessMat)
			{
				UVOffsetMID = UMaterialInstanceDynamic::Create(PostProcessMat, this);

				// Post Process Volume의 배열에 MID로 다시 설정해야 적용
				PPVolume->Settings.WeightedBlendables.Array[0].Object = UVOffsetMID;
			}
		}
	}

	if (UVOffsetMID)
	{
		// 기존 타이머가 있다면 해제하고 CurrentOffsetTime 초기화
		if (OffsetTimerHandle.IsValid())
		{
			// 기존 타이머를 클리어 (활성화 여부와 상관없이 안전하게 초기화)
			GetWorld()->GetTimerManager().ClearTimer(OffsetTimerHandle);
		}

		CurrentOffsetTime = 0.0f;

		// 타이머 설정: 0.01초마다 UpdateOffsetEffect 함수를 반복 호출
		const float TickInterval = 0.01f;

		GetWorld()->GetTimerManager().SetTimer(
			OffsetTimerHandle,          // 타이머 핸들
			this,                       // 타이머가 호출할 객체
			&UARGC_SpecialAttack::UpdateOffsetEffect, // 호출할 함수
			TickInterval,               // 반복 간격 (매 0.01초)
			true                        // 반복(Loop) 설정
		);
	}
}

void UARGC_SpecialAttack::UpdateOffsetEffect()
{
	// 매 프레임의 DeltaTime 대신, 타이머 설정 시 사용한 간격(0.01초)을 사용
	const float TickInterval = 0.01f;

	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Timer Update")); // 화면출력

	// 현재 진행 시간을 누적
	CurrentOffsetTime += TickInterval;

	// 전체 지속 시간에 대한 현재 진행 비율 (0.0 -> 1.0)
	float TimeRatio = CurrentOffsetTime / OffsetDuration;

	// 만약 TimeRatio가 1.0을 초과하면 애니메이션을 종료하고 강제로 1.0으로 설정
	if (TimeRatio >= 1.0f)
	{
		TimeRatio = 1.0f;
	}

	// 0.0 -> 0.5f -> 0.0f 로직 구현
	float NewOffsetValue = 0.0f;

	if (TimeRatio <= 0.5f)
	{
		// 0.0 -> 0.5까지 상승 (LerpAlpha는 0.0f -> 1.0f로 조정)
		float LerpAlpha = TimeRatio * 2.0f;
		NewOffsetValue = FMath::Lerp(0.0f, 0.3f, LerpAlpha);
	}
	else // TimeRatio > 0.5f
	{
		// 0.5 -> 0.0까지 하강 (LerpAlpha는 0.0f -> 1.0f로 조정)
		float LerpAlpha = (TimeRatio - 0.5f) * 2.0f;
		NewOffsetValue = FMath::Lerp(0.3f, 0.0f, LerpAlpha);
	}

	// 머티리얼 파라미터 업데이트
	UVOffsetMID->SetScalarParameterValue(FName("Offset"), NewOffsetValue);


	// 애니메이션 완료 시 타이머 정지
	if (CurrentOffsetTime >= OffsetDuration)
	{
		// 최종적으로 0.0f로 설정
		UVOffsetMID->SetScalarParameterValue(FName("Offset"), 0.0f);
		FinishOffsetEffect();
	}
}

void UARGC_SpecialAttack::FinishOffsetEffect()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("FinishOffsetEffect")); // 화면출력

	if (UVOffsetMID)
	{
		UVOffsetMID->SetScalarParameterValue(FName("Offset"), 0.0f);
	}

	// 타이머를 해제.
	GetWorld()->GetTimerManager().ClearTimer(OffsetTimerHandle);

	// 타이머 핸들을 비유효(Invalid) 상태로 만듦.
	OffsetTimerHandle.Invalidate();
}
