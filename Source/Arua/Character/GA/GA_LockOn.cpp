// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_LockOn.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tag/AruaGameplayTags.h"

UGA_LockOn::UGA_LockOn()
{
    // Ability 기본 설정
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

    // 입력이 Release될 때까지 Ability가 유지되도록 설정
    bRetriggerInstancedAbility = false;

    AbilityTags.AddTag(TAG_PLAYER_STATE_LOCKON);
}

void UGA_LockOn::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // 타겟 찾기
    AActor* Target = FindLockOnTarget();

    if (Target)
    {
        StartLockOn(Target);
    }
    else
    {
        // 타겟이 없으면 Ability 종료
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    }
}

void UGA_LockOn::InputReleased(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo)
{
    if (ActorInfo != nullptr && ActorInfo->AvatarActor != nullptr)
    {
        // 입력이 Release되면 Lock-On 종료
        EndLockOn();
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    }
}

void UGA_LockOn::CancelAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateCancelAbility)
{
    EndLockOn();
    Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

AActor* UGA_LockOn::FindLockOnTarget()
{
    ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
    if (!Character)
        return nullptr;

    FVector CharacterLocation = Character->GetActorLocation();
    FVector ForwardVector = Character->GetActorForwardVector();

    // 주변 적 탐색
    TArray<AActor*> FoundActors;
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(Character);

    UKismetSystemLibrary::SphereOverlapActors(
        GetWorld(),
        CharacterLocation,
        LockOnRange,
        ObjectTypes,
        TargetActorClass,
        ActorsToIgnore,
        FoundActors
    );

    AActor* BestTarget = nullptr;
    float BestScore = -1.0f;

    for (AActor* Actor : FoundActors)
    {
        FVector ToTarget = Actor->GetActorLocation() - CharacterLocation;
        ToTarget.Normalize();

        // 각도 체크
        float DotProduct = FVector::DotProduct(ForwardVector, ToTarget);
        float Angle = FMath::Acos(DotProduct) * (180.0f / PI);

        if (Angle <= LockOnAngle)
        {
            // 각도와 거리를 고려한 점수 계산
            float Distance = FVector::Dist(CharacterLocation, Actor->GetActorLocation());
            float Score = (1.0f - (Angle / LockOnAngle)) * (1.0f - (Distance / LockOnRange));

            if (Score > BestScore)
            {
                BestScore = Score;
                BestTarget = Actor;
            }
        }
    }

    return BestTarget;
}

void UGA_LockOn::StartLockOn(AActor* Target)
{
    CurrentTarget = Target;

    // GameplayTag 적용 (선택사항)
    if (LockOnStateTag.IsValid())
    {
        GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(LockOnStateTag);
    }

    // 여기서 Lock-On UI 표시, 카메라 조정 등을 BP에서 처리할 수 있도록 이벤트 발생
    // 또는 C++로 직접 처리

    UE_LOG(LogTemp, Log, TEXT("Lock-On Started: %s"), *Target->GetName());
}

void UGA_LockOn::EndLockOn()
{
    if (CurrentTarget)
    {
        UE_LOG(LogTemp, Log, TEXT("Lock-On Ended: %s"), *CurrentTarget->GetName());
        CurrentTarget = nullptr;
    }

    // GameplayTag 제거
    if (LockOnStateTag.IsValid())
    {
        GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(LockOnStateTag);
    }

    // Lock-On UI 제거 등
}