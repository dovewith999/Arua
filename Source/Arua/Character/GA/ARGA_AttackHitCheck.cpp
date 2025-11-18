// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGA_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/AbilityTask/ARAT_Trace.h"
#include "Character/GA/TA/ARTA_TraceBase.h"
#include "Util/DamageLibrary.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "Tag/AruaGameplayTags.h"
#include "Character/ARCharacterPlayer.h"
#include "Interface/ARHitReactableInterface.h"

UARGA_AttackHitCheck::UARGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UARGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	// OptionalObject에서 클래스 정보를 추출
	const UClass* TargetClass = Cast<UClass>(TriggerEventData->OptionalObject.Get());

	TSubclassOf<AARTA_TraceBase> TargetActorClass = nullptr;

	if (TargetClass && TargetClass->IsChildOf(AARTA_TraceBase::StaticClass()))
	{
		TargetActorClass = TSubclassOf<AARTA_TraceBase>(const_cast<UClass*>(TargetClass));
	}
	
	//Todo : 호출시킨 공격 태그를 가져오도록 설정
	UARAT_Trace* AttackTraceTask = UARAT_Trace::CreateTask(this, TargetActorClass);
	AttackTraceTask->OnComplete.AddDynamic(this, &UARGA_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}

void UARGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle,0))
	{
		FHitResult HitResult= UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		UE_LOG(LogTemp, Log, TEXT("Hit %s"), *HitResult.GetActor()->GetName());

		//Todo : 태그 비교해서 같은 태그를 가지고 있으면 처리 x
		
		// 데미지 여기서 처리
		UDamageLibrary::ApplyDamage(CurrentActorInfo->AbilitySystemComponent.Get(), HitResult.GetActor());

		AActor* HitActor = HitResult.GetActor();

		// HitActor가 ARHitReactableInterface를 구현했는지 확인.
		if (HitActor && HitActor->Implements<UARHitReactableInterface>())
		{
			ACharacter* Instigator = Cast<ACharacter>(GetAvatarActorFromActorInfo());

			// UHT가 생성한 Execute_ 함수를 호출하여 Blueprint 및 C++ 구현을 모두 처리.
			IARHitReactableInterface::Execute_OnHitByAttack(HitActor, HitResult, Instigator);
		}
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);


}
