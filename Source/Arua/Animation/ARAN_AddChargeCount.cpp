// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ARAN_AddChargeCount.h"
#include "AbilitySystemBlueprintLibrary.h"

UARAN_AddChargeCount::UARAN_AddChargeCount()
{
}

FString UARAN_AddChargeCount::GetNotifyName_Implementation() const
{
	return TEXT("AddChrageCount");
}

void UARAN_AddChargeCount::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			FGameplayEventData PayloadData;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
		}
	}
}
