// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AbilityTask/ARAT_Trace.h"
#include "Character/GA/TA/ARTA_Trace.h"
#include "AbilitySystemComponent.h"

UARAT_Trace::UARAT_Trace()
{
}

UARAT_Trace* UARAT_Trace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<AARTA_Trace> TargetActorClass)
{
	UARAT_Trace* NewTask = NewAbilityTask<UARAT_Trace>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;

	return NewTask;
}

void UARAT_Trace::Activate()
{
	Super::Activate();

	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UARAT_Trace::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UARAT_Trace::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<AARTA_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass,FTransform::Identity,nullptr,nullptr,ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetShowDebug(true);
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UARAT_Trace::OnTargetDataReadyCallback);
	}
}

void UARAT_Trace::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);
		SpawnedTargetActor->ConfirmTargeting();
	}
}

void UARAT_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}