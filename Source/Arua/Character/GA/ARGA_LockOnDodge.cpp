// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGA_LockOnDodge.h"
#include "Character/ARCharacterPlayer.h"
#include "Tag/AruaGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"
#include "Animation/ARCharacterAnimInstance.h"

UARGA_LockOnDodge::UARGA_LockOnDodge()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	// 어빌리티 식별 Tag
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(AruaGamePlayTags::Ability_LockOnDodge);
	SetAssetTags(TagContainer);

	// 활성화 시 Owner에게 부여되는 Tag
	ActivationOwnedTags.AddTag(AruaGamePlayTags::Player_State_Roll);
	ActivationOwnedTags.AddTag(AruaGamePlayTags::Condition_Immunity);
}

void UARGA_LockOnDodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AARCharacterPlayer* Player = CastChecked<AARCharacterPlayer>(ActorInfo->AvatarActor.Get());
	Player->SetInputDirection();

	UARCharacterAnimInstance* PlayerAnim = Cast<UARCharacterAnimInstance>(Player->GetMesh()->GetAnimInstance());
	UAnimMontage* DodgeMontage = PlayerAnim->FindMontageInternal(Player->GetWeaponTag(), (FGameplayTag::RequestGameplayTag("Character.Action.LockOnSlide")));
	if (DodgeMontage)
	{
		UE_LOG(LogTemp, Log, TEXT("Montage Found!"));
	}

	UE_LOG(LogTemp, Log, TEXT("%s"), *Player->GetLockOnDodgeMontageSection().ToString());
	UAbilityTask_PlayMontageAndWait* PlayRollTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("LockOnRoll"), DodgeMontage, 1.f, Player->GetLockOnDodgeMontageSection());
	PlayRollTask->OnCompleted.AddDynamic(this, &UARGA_LockOnDodge::OnCompleteCallback);
	PlayRollTask->OnInterrupted.AddDynamic(this, &UARGA_LockOnDodge::OnInterruptedCallback);
	PlayRollTask->ReadyForActivation();
}

void UARGA_LockOnDodge::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UARGA_LockOnDodge::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UARGA_LockOnDodge::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UARGA_LockOnDodge::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}