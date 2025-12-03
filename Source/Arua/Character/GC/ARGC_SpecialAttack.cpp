// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGC_SpecialAttack.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/PostProcessVolume.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AARGC_SpecialAttack::AARGC_SpecialAttack()
{
	// 효과 종료 시 자동으로 액터 제거
	// bAutoDestroyOnRemove = true;

	// 기본값 초기화
	UVOffsetMID = nullptr;
	CurrentOffsetTime = 0.0f;
}

bool AARGC_SpecialAttack::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters)
{
	// 사운드 재생
	if (EffectSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			Target->GetWorld(),
			EffectSound,
			Target->GetActorLocation(),
			Target->GetActorRotation(),
			SoundVolume,
			SoundPitch,
			0.0f,  // StartTime
			nullptr,  // AttenuationSettings (기본값 사용)
			nullptr,  // ConcurrencySettings
			Target  // Owner
		);
	}

	StartOffsetEffect();

	return true;
}

bool AARGC_SpecialAttack::OnRemove_Implementation(AActor* Target, const FGameplayCueParameters& Parameters)
{
	// 효과 종료 및 MID 정리
	FinishOffsetEffect();

	return Super::OnRemove_Implementation(Target, Parameters);
}

void AARGC_SpecialAttack::StartOffsetEffect()
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

	// 머티리얼 인스턴스 찾기 및 MID 생성
	// Post Process Volume의 Settings Array에서 머티리얼을 찾아야 함
	FWeightedBlendables Blendables = PPVolume->Settings.WeightedBlendables;

	if (Blendables.Array.Num() > 0)
	{
		PostProcessMat = Cast<UMaterialInterface>(Blendables.Array[0].Object);

		// UMaterialInstanceDynamic으로 변환
		if (PostProcessMat)
		{
			UVOffsetMID = UMaterialInstanceDynamic::Create(PostProcessMat, this);

			// Post Process Volume의 배열에 MID로 다시 설정해야 적용
			PPVolume->Settings.WeightedBlendables.Array[0].Object = UVOffsetMID;
		}
	}

	if (UVOffsetMID)
	{
		if (OffsetTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(OffsetTimerHandle);
		}

		CurrentOffsetTime = 0.0f;

		const float TickInterval = 0.01f;

		GetWorld()->GetTimerManager().SetTimer(
			OffsetTimerHandle,          // 타이머 핸들
			this,                       // 타이머가 호출할 객체
			&AARGC_SpecialAttack::UpdateOffsetEffect, // 호출할 함수
			TickInterval,               // 반복 간격 (매 0.01초)
			true                        // 반복(Loop) 설정
		);
	}
}

void AARGC_SpecialAttack::UpdateOffsetEffect()
{
	const float TickInterval = 0.01f;

	CurrentOffsetTime += TickInterval;

	// 전체 지속 시간에 대한 현재 진행 비율 
	float TimeRatio = CurrentOffsetTime / OffsetDuration;

	if (TimeRatio >= 1.0f)
	{
		TimeRatio = 1.0f;
	}

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
		FinishOffsetEffect();
	}
}

void AARGC_SpecialAttack::FinishOffsetEffect()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("FinishOffsetEffect")); // 화면출력
	if (UVOffsetMID)
	{
		// Offset 값을 0으로 초기화
		UVOffsetMID->SetScalarParameterValue(FName("Offset"), 0.0f);

		APostProcessVolume* PPVolume = nullptr;
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APostProcessVolume::StaticClass(), FoundActors);

		if (FoundActors.Num() > 0)
		{
			PPVolume = Cast<APostProcessVolume>(FoundActors[0]);
		}

		if (PPVolume->Settings.WeightedBlendables.Array.Num() == 0)
		{
			return;
		}

		PPVolume->Settings.WeightedBlendables.Array[0].Object = PostProcessMat;
	}

	// 타이머 정리
	if (GetWorld() && OffsetTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(OffsetTimerHandle);
		OffsetTimerHandle.Invalidate();

		UE_LOG(LogTemp, Log, TEXT("Timer Cleared"));
	}
}
