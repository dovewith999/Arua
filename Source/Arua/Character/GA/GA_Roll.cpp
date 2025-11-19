// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GA/GA_Roll.h"
#include "Tag/AruaGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/ARCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Animation/ARCharacterAnimInstance.h"

UGA_Roll::UGA_Roll()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	// 어빌리티 식별 Tag
	//AbilityTags.AddTag(AruaGamePlayTags::Ability_Roll);
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(AruaGamePlayTags::Ability_Roll);
	SetAssetTags(TagContainer);

	// 활성화 시 Owner에게 부여되는 Tag
	ActivationOwnedTags.AddTag(AruaGamePlayTags::Player_State_Roll);
	ActivationOwnedTags.AddTag(AruaGamePlayTags::Condition_Immunity);
}

void UGA_Roll::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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
	UAnimMontage* RollMontage = PlayerAnim->FindMontageInternal(Player->GetWeaponTag(), (FGameplayTag::RequestGameplayTag("Character.Action.Roll")));

	UAbilityTask_PlayMontageAndWait* PlayRollTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayRoll"), RollMontage);
	PlayRollTask->OnCompleted.AddDynamic(this, &UGA_Roll::OnCompleteCallback);
	PlayRollTask->OnInterrupted.AddDynamic(this, &UGA_Roll::OnInterruptedCallback);
	PlayRollTask->ReadyForActivation();
}

void UGA_Roll::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	//if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
	//{
	//	FGameplayTagContainer TagContainer;
	//	TagContainer.AddTag(AruaGamePlayTags::Player_State_Roll);
	//	TagContainer.AddTag(AruaGamePlayTags::Condition_Immunity);
	//}

	K2_EndAbility();
}

void UGA_Roll::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Roll::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UGA_Roll::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}