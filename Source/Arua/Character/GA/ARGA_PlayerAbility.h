// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ARGA_PlayerAbility.generated.h"

/**
 * Player가 이용하는 GA의 부모가 될 클래스
 * 작성자 : 임희섭
 * 작성일 : 25/11/13
 */
UCLASS()
class ARUA_API UARGA_PlayerAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled
	) override;
	
protected:
	UPROPERTY();
	TObjectPtr<class AARCharacterPlayer> Player;
};
