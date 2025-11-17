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

		if (HitResult.GetActor()->ActorHasTag(FName("Player")))
		{
			ACharacter* TargetCharacter = Cast<ACharacter>(HitResult.GetActor());
			ACharacter* Instigator = Cast<ACharacter>(GetAvatarActorFromActorInfo());

			if (TargetCharacter && Instigator)
			{
				
				FVector ToInstigator = (Instigator->GetActorLocation() - TargetCharacter->GetActorLocation()).GetSafeNormal2D();
				FVector TargetForward = TargetCharacter->GetActorForwardVector();

				float Dot = FVector::DotProduct(TargetForward, ToInstigator);
				float CrossZ = FVector::CrossProduct(TargetForward, ToInstigator).Z;

				UE_LOG(LogTemp, Log, TEXT("%f"), Dot);
				FGameplayTag HitCueTag;

				if (Dot > 0.7f) HitCueTag = FGameplayTag::RequestGameplayTag("GameplayCue.Hit.Front");
				else if (Dot < -0.7f) HitCueTag = FGameplayTag::RequestGameplayTag("GameplayCue.Hit.Back");
				else if (CrossZ > 0) HitCueTag = FGameplayTag::RequestGameplayTag("GameplayCue.Hit.Left");
				else HitCueTag = FGameplayTag::RequestGameplayTag("GameplayCue.Hit.Right");

				FGameplayCueParameters Params;
				Params.Instigator = Instigator;
				Params.EffectCauser = Instigator;
				Params.Location = HitResult.ImpactPoint; 
				Params.SourceObject = HitResult.GetActor();
				Params.Normal = HitResult.ImpactNormal;

				Params.AggregatedTargetTags.AddTag(HitCueTag);
				if (Params.AggregatedTargetTags.HasTag(HitCueTag))
				{
					UE_LOG(LogTemp, Log, TEXT("HitCueTag is in AggregatedTargetTags!"));
				}
				else
				{
					UE_LOG(LogTemp, Log, TEXT(" HitCueTag is NOT in AggregatedTargetTags!"));
				}

				Cast<AARCharacterPlayer>(HitResult.GetActor())->GetAbilitySystemComponent()->ExecuteGameplayCue(AruaGamePlayTags::GameplayCue_Character_AttackHit, Params);
			}
		}

		else
		{
			ACharacter* TargetCharacter = Cast<ACharacter>(HitResult.GetActor());
			ACharacter* Instigator = Cast<ACharacter>(GetAvatarActorFromActorInfo());

			if (TargetCharacter && Instigator)
			{
				// GameplayCue 파라미터 설정
				FGameplayCueParameters CueParams;
				CueParams.Instigator = Instigator;
				CueParams.EffectCauser = Instigator;
				CueParams.Location = HitResult.ImpactPoint;
				CueParams.Normal = HitResult.ImpactNormal;
				CueParams.SourceObject = HitResult.GetActor();

				// 몬스터 ASC 가져오기
				UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetCharacter);

				if (TargetASC)
				{
					// 몬스터 Hit Cue 실행
					TargetASC->ExecuteGameplayCue(
						AruaGamePlayTags::GameplayCue_Monster_Hit,
						CueParams
					);
				}
			}
		}
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);


}
