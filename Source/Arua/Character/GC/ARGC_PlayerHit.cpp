// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GC/ARGC_PlayerHit.h"
#include "GameFramework/Character.h"

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

    ACharacter* Character = Cast<ACharacter>(Target);

    UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
    if (AnimInstance)
    {
        AnimInstance->Montage_Play(HitActionMontage,1.0f);
        //if (!Character->GetMesh())
        //{
        //    UE_LOG(LogTemp, Error, TEXT("[MontageDebug] MeshComp is NULL"));
        //    return false;
        //}

        //if (!AnimInstance)
        //{
        //    UE_LOG(LogTemp, Error, TEXT("[MontageDebug] AnimInstance is NULL (AnimBlueprint not assigned?)"));
        //    return false;
        //}

        //if (!HitActionMontage)
        //{
        //    UE_LOG(LogTemp, Error, TEXT("[MontageDebug] Montage is NULL"));
        //    return false;
        //}

        //// 스켈레톤 호환 여부 확인
        //if (HitActionMontage->GetSkeleton() != Character->GetMesh()->SkeletalMesh->GetSkeleton())
        //{
        //    UE_LOG(LogTemp, Error, TEXT("[MontageDebug] Skeleton mismatch! Montage vs Mesh"));
        //    return false;
        //}

 

        //// 재생 시도
        //float Duration = AnimInstance->Montage_Play(HitActionMontage, 1.0f);
        //if (Duration <= 0.f)
        //{
        //    UE_LOG(LogTemp, Error, TEXT("[MontageDebug] Montage_Play returned 0. Montage may have no valid sequence or sections."));
        //    return false;
        //}

        //

        //// 첫 섹션 이름 출력
        //FName FirstSection = HitActionMontage->GetSectionName(0);
        //UE_LOG(LogTemp, Log, TEXT("[MontageDebug] First Section: %s"), *FirstSection.ToString());
        if (Parameters.AggregatedTargetTags.HasTag(FGameplayTag::RequestGameplayTag("GameplayCue.Hit.Front")))
        {
            UE_LOG(LogTemp, Log, TEXT("FrontOK"));
            if (AnimInstance->Montage_IsPlaying(HitActionMontage))
            {
                // 현재 재생 중이면 해당 섹션으로 점프
                AnimInstance->Montage_JumpToSection("Front", HitActionMontage);
            }
        }
        else if (Parameters.AggregatedTargetTags.HasTag(FGameplayTag::RequestGameplayTag("GameplayCue.Hit.Back")))
        {
            UE_LOG(LogTemp, Log, TEXT("BackOK"));
            if (AnimInstance->Montage_IsPlaying(HitActionMontage))
            {
                // 현재 재생 중이면 해당 섹션으로 점프
                AnimInstance->Montage_JumpToSection("Back", HitActionMontage);
            }
        }
        else if (Parameters.AggregatedTargetTags.HasTag(FGameplayTag::RequestGameplayTag("GameplayCue.Hit.Right")))
        {
            UE_LOG(LogTemp, Log, TEXT("RightOK"));
            if (AnimInstance->Montage_IsPlaying(HitActionMontage))
            {
                // 현재 재생 중이면 해당 섹션으로 점프
                AnimInstance->Montage_JumpToSection("Right", HitActionMontage);
            }
        }
        else if (Parameters.AggregatedTargetTags.HasTag(FGameplayTag::RequestGameplayTag("GameplayCue.Hit.Left")))
        {
            UE_LOG(LogTemp, Log, TEXT("LeftOK"));
            if (AnimInstance->Montage_IsPlaying(HitActionMontage))
            {
                // 현재 재생 중이면 해당 섹션으로 점프
                AnimInstance->Montage_JumpToSection("Left", HitActionMontage);
            }
        }
    }
    return false;
   /* if (!Target) return false;*/

    /*const FGameplayTag FrontTag = FGameplayTag::RequestGameplayTag("GameplayCue.Hit.Front");
    const FGameplayTag BackTag = FGameplayTag::RequestGameplayTag("GameplayCue.Hit.Back");
    const FGameplayTag LeftTag = FGameplayTag::RequestGameplayTag("GameplayCue.Hit.Left");
    const FGameplayTag RightTag = FGameplayTag::RequestGameplayTag("GameplayCue.Hit.Right");

    const FGameplayTagContainer& TargetTags = Parameters.AggregatedTargetTags;
    AnimInstance->Montage_Play(HitActionMontage, 1.0f);
    if (TargetTags.HasTagExact(FrontTag))
    {
        AnimInstance->Montage_JumpToSection("Front", HitActionMontage);
    }
    else if (TargetTags.HasTagExact(BackTag))
    {
        AnimInstance->Montage_JumpToSection("Back", HitActionMontage);
    }
    else if (TargetTags.HasTagExact(LeftTag))
    {
        AnimInstance->Montage_JumpToSection("Right", HitActionMontage);
    }
    else if (TargetTags.HasTagExact(RightTag))
    {
        AnimInstance->Montage_JumpToSection("Left", HitActionMontage);
    }

    return true;*/

}
