// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGA_Death.h"
#include "Character/ARCharacterBase.h"
#include "Tag/AruaGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"



UARGA_Death::UARGA_Death()
{
}

void UARGA_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (Avator = CastChecked<AARCharacterBase>(ActorInfo->AvatarActor.Get()))
	{
		//UAbilityTask_PlayMontageAndWait* DeathTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("DeathTask"), Avator->GetDeathMontage());

		//DeathTask->OnCompleted.AddDynamic(this, &UARGA_Death::OnDeathCallback);
	
		//DeathTask->ReadyForActivation();
	}
}

void UARGA_Death::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UARGA_Death::OnDeathCallback()
{
	// TODO : 죽음 처리
	//Avator->SetDaad();
}
