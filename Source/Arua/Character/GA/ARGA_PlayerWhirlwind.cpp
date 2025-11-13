// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GA/ARGA_PlayerWhirlwind.h"
#include "Tag/AruaGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/ARCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

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

}

void UARGA_PlayerWhirlwind::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Player = CastChecked<AARCharacterPlayer>(ActorInfo->AvatarActor.Get());

	UAbilityTask_PlayMontageAndWait* PlayRollTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayWhirlwind"), Player->GetSkillMontage(AruaGamePlayTags::Ability_Whirlwind));
	PlayRollTask->OnCompleted.AddDynamic(this, &UARGA_PlayerWhirlwind::OnCompleteCallback);
	PlayRollTask->ReadyForActivation();

	if (USkeletalMeshComponent* SkelMesh = Player->GetMesh())
	{
		if (UAnimInstance* AnimInstance = SkelMesh->GetAnimInstance())
		{
			AnimInstance->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
		}
	}
}

void UARGA_PlayerWhirlwind::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
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
	K2_EndAbility();
}