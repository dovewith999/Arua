// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GA/ARGA_PlayerChargeAttack.h"
#include "Character/ARCharacterPlayer.h"
#include "Tag/AruaGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet/PlayerAttributeSet.h"

UARGA_PlayerChargeAttack::UARGA_PlayerChargeAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	// 어빌리티 식별 Tag
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(AruaGamePlayTags::Ability_ChargeAttack);
	SetAssetTags(TagContainer);

	// 활성화 시 Owner에게 부여되는 Tag
	ActivationOwnedTags.AddTag(AruaGamePlayTags::Player_State_Skill_ChargeAttack);
}

void UARGA_PlayerChargeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (ChargeAttackMontage == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilityTask_PlayMontageAndWait* PlayWhirlwindTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayChargeAttack"), ChargeAttackMontage);
	PlayWhirlwindTask->OnCompleted.AddDynamic(this, &UARGA_PlayerChargeAttack::OnCompleteCallback);
	PlayWhirlwindTask->OnInterrupted.AddDynamic(this, &UARGA_PlayerChargeAttack::OnInterruptedCallback);
	PlayWhirlwindTask->ReadyForActivation();
}

void UARGA_PlayerChargeAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility, true);
}

void UARGA_PlayerChargeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	UPlayerAttributeSet* PlayerAttributeSet = const_cast<UPlayerAttributeSet*>(Cast<UPlayerAttributeSet>(
		Player->GetAbilitySystemComponent()->GetAttributeSet(UPlayerAttributeSet::StaticClass())));

	//const UPlayerAttributeSet* PlayerAttributeSet = Cast<UPlayerAttributeSet>(
	//	Player->GetAbilitySystemComponent()->GetAttributeSet(UPlayerAttributeSet::StaticClass()));

	PlayerAttributeSet->SetChargeCount(0.f);
}

void UARGA_PlayerChargeAttack::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	const UPlayerAttributeSet* PlayerAttributeSet = Cast<UPlayerAttributeSet>(
		Player->GetAbilitySystemComponent()->GetAttributeSet(UPlayerAttributeSet::StaticClass()));

	if (PlayerAttributeSet)
	{
		// ChargeCount 값을 float에서 int로 가져옵니다. (필요하다면 FMath::RoundToInt 사용)
		int32 ChargeCount = FMath::RoundToInt(PlayerAttributeSet->GetChargeCount());

		// 2. ChargeCount를 사용하여 동적으로 섹션 이름 생성
		// FName SectionName = FString::Printf(TEXT("Attack%d"), ChargeCount); 
		// FName으로 변환할 때 FString을 사용하는 것이 더 안전합니다.
		FString SectionNameString = FString::Printf(TEXT("Attack%d"), ChargeCount + 1);
		FName SectionName = FName(*SectionNameString);

		// 3. 생성된 동적 섹션 이름으로 점프 실행
		// MontageJumpToSection(FName("Attack1")); 대신 SectionName을 사용합니다.
		MontageJumpToSection(SectionName);
	}
	else
	{
		// AttributeSet을 가져오지 못한 경우 기본 섹션으로 점프하거나 로그 출력
		MontageJumpToSection(FName("Attack1"));
		UE_LOG(LogTemp, Warning, TEXT("Failed to get UPlayerAttributeSet for Charge Attack."));
	}
}

void UARGA_PlayerChargeAttack::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UARGA_PlayerChargeAttack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}