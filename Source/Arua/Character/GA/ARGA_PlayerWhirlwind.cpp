// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GA/ARGA_PlayerWhirlwind.h"
#include "Tag/AruaGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/ARCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"

UARGA_PlayerWhirlwind::UARGA_PlayerWhirlwind()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	// 어빌리티 식별 Tag
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(AruaGamePlayTags::Ability_Whirlwind);
	SetAssetTags(TagContainer);

	// 활성화 시 Owner에게 부여되는 Tag
	ActivationOwnedTags.AddTag(AruaGamePlayTags::Player_State_Skill_Whirlwind);
	
	// 활성화 중에 Block 되는 Ability Tag
	BlockAbilitiesWithTag.AddTag(AruaGamePlayTags::Player_State_Skill_Whirlwind);
	BlockAbilitiesWithTag.AddTag(AruaGamePlayTags::Player_State_Skill_ChargeAttack);
}

void UARGA_PlayerWhirlwind::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	//Player = CastChecked<AARCharacterPlayer>(ActorInfo->AvatarActor.Get());

	UAbilityTask_PlayMontageAndWait* PlayWhirlwindTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayWhirlwind"), Player->GetSkillMontage(AruaGamePlayTags::Ability_Whirlwind));
	PlayWhirlwindTask->OnCompleted.AddDynamic(this, &UARGA_PlayerWhirlwind::OnCompleteCallback);
	PlayWhirlwindTask->OnInterrupted.AddDynamic(this, &UARGA_PlayerWhirlwind::OnInterruptedCallback);
	PlayWhirlwindTask->ReadyForActivation();

	if (USkeletalMeshComponent* SkelMesh = Player->GetMesh())
	{
		if (UAnimInstance* AnimInstance = SkelMesh->GetAnimInstance())
		{
			AnimInstance->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
		}
	}
}

void UARGA_PlayerWhirlwind::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility, true);
}

void UARGA_PlayerWhirlwind::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (USkeletalMeshComponent* SkelMesh = Player->GetMesh())
	{
		if (UAnimInstance* AnimInstance = SkelMesh->GetAnimInstance())
		{
			AnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
		}
	}
}

void UARGA_PlayerWhirlwind::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	MontageJumpToSection(FName("End_Attack"));

	if (USkeletalMeshComponent* SkelMesh = Player->GetMesh())
	{
		if (UAnimInstance* AnimInstance = SkelMesh->GetAnimInstance())
		{
			AnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
		}
	}
}

void UARGA_PlayerWhirlwind::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UARGA_PlayerWhirlwind::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}