// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GA/ARGA_PlayerAddChargeCount.h"
#include "Character/ARCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Tag/AruaGameplayTags.h"

UARGA_PlayerAddChargeCount::UARGA_PlayerAddChargeCount()
{
	// 어빌리티 식별 Tag
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(AruaGamePlayTags::Ability_AddChargeCount);
	SetAssetTags(TagContainer);
}

void UARGA_PlayerAddChargeCount::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (AddChargeEffectClass == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FGameplayEffectSpecHandle AddChargeEffectSpec = MakeOutgoingGameplayEffectSpec(AddChargeEffectClass);
	Player->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*AddChargeEffectSpec.Data.Get());
}

void UARGA_PlayerAddChargeCount::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility, true);
}

void UARGA_PlayerAddChargeCount::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
