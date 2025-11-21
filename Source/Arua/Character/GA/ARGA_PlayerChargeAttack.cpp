// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GA/ARGA_PlayerChargeAttack.h"
#include "Character/ARCharacterPlayer.h"
#include "Tag/AruaGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet/PlayerAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Weapon/ARSword.h"

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

	// 활성화 중에 Block 되는 Ability Tag
	BlockAbilitiesWithTag.AddTag(AruaGamePlayTags::Ability_Whirlwind);
	BlockAbilitiesWithTag.AddTag(AruaGamePlayTags::Ability_ChargeAttack);
}

void UARGA_PlayerChargeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Activate ChargeAttack"));

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

	bInputReleaseHandled = false;

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

	PlayerAttributeSet->SetChargeCount(0.f);

	if (AARSword* Weapon = Cast<AARSword>(Player->GetCurrentWeapon()))
	{
		Weapon->InitVisible();
	}
}

void UARGA_PlayerChargeAttack::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Ability Input Released"));

	if (bInputReleaseHandled)
	{
		return;
	}
		
	bInputReleaseHandled = true;

	const UPlayerAttributeSet* PlayerAttributeSet = Cast<UPlayerAttributeSet>(
		Player->GetAbilitySystemComponent()->GetAttributeSet(UPlayerAttributeSet::StaticClass()));

	if (PlayerAttributeSet)
	{
		int32 ChargeCount = FMath::RoundToInt(PlayerAttributeSet->GetChargeCount());

		// ChargeCount를 사용하여 동적으로 섹션 이름 생성
		FString SectionNameString = FString::Printf(TEXT("Attack%d"), ChargeCount + 1);
		FName SectionName = FName(*SectionNameString);

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