// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GC/ARGC_PlayerHit.h"

UARGC_PlayerHit::UARGC_PlayerHit()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitActionMontageRef(TEXT("/Game/Animation/Player/AM_Hit.AM_Hit"));
	if (HitActionMontageRef.Object)
	{
		HitActionMontage = HitActionMontageRef.Object;
	}
}

bool UARGC_PlayerHit::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
    //if (UAnimInstance* AnimInstance = MyCharacter->GetMesh()->GetAnimInstance())
    //{
    //    if (Parameters.AggregatedSourceTags.HasTagExact(FGameplayTag::RequestGameplayTag("GameplayCue.Hit.Front")))
    //    {
    //        if (AnimInstance->Montage_IsPlaying(Montage))
    //        {
    //            // 현재 재생 중이면 해당 섹션으로 점프
    //            AnimInstance->Montage_JumpToSection(SectionName, Montage);
    //        }
    //    }
    //    else if (Parameters.AggregatedSourceTags.HasTagExact(FGameplayTag::RequestGameplayTag("GameplayCue.Hit.Back")))
    //    {
    //        UE_LOG(LogTemp, Log, TEXT("Hit from Back"));
    //    }
    //    else if (Parameters.AggregatedSourceTags.HasTagExact(FGameplayTag::RequestGameplayTag("GameplayCue.Hit.Right")))
    //    {
    //        UE_LOG(LogTemp, Log, TEXT("Hit from Back"));
    //    }
    //    else if (Parameters.AggregatedSourceTags.HasTagExact(FGameplayTag::RequestGameplayTag("GameplayCue.Hit.Left")))
    //    {
    //        UE_LOG(LogTemp, Log, TEXT("Hit from Back"));
    //    }
    //}
    return true;
}
