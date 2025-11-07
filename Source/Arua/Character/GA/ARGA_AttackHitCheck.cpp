// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGA_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/AbilityTask/ARAT_Trace.h"
#include "Character/GA/TA/ARTA_Trace.h"
#include "Util/DamageLibrary.h"

UARGA_AttackHitCheck::UARGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UARGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	UARAT_Trace* AttackTraceTask = UARAT_Trace::CreateTask(this, AARTA_Trace::StaticClass());
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
		
		// Todo : 데미지 여기서 처리
		//Damager->ApplyDamage(HitResult.GetActor(), 10.f, 2.f);

		UDamageLibrary::ApplyDamage(CurrentActorInfo->AbilitySystemComponent.Get(), HitResult.GetActor());
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);


}
