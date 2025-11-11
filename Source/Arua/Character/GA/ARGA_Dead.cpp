// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGA_Dead.h"
#include "Character/ARCharacterBase.h"
#include "Tag/AruaGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UARGA_Dead::UARGA_Dead()
{
	// 활성화 시 Owner에게 부여되는 Tag
	// ActivationOwnedTags.AddTag(AruaGamePlayTags::Event_Dead);
}

void UARGA_Dead::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (Avator = CastChecked<AARCharacterBase>(ActorInfo->AvatarActor.Get()))
	{
		UAbilityTask_PlayMontageAndWait* DeathTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("DeadTask"), Avator->GetDeadMontage());

		DeathTask->OnCompleted.AddDynamic(this, &UARGA_Dead::OnDeadCallback);

		DeathTask->ReadyForActivation();

		Avator->SetDead();
	}
}

void UARGA_Dead::OnDeadCallback()
{
    // 어빌리티 종료
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UARGA_Dead::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
