// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Item/DA_QuestItemDefinition.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "Tag/AruaGameplayTags.h"

void UDA_QuestItemDefinition::ApplyEffect_Implementation(AActor* User)
{
	// 퀘스트 아이템 사용 시, 플레이어에게 퀘스트 업데이트 이벤트 전달
	if (UAbilitySystemComponent* OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(User))
	{
		FGameplayEventData Payload;
		Payload.OptionalObject = this;
		Payload.TargetTags = FGameplayTagContainer(AruaGamePlayTags::QuestItem);

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(User, AruaGamePlayTags::Event_Quest_Update, Payload);
	}

	return;
}
