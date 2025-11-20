// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/TagRemoveAnimNotify.h"
#include "AbilitySystemComponent.h"
#include "Character/ARCharacterPlayer.h"

void UTagRemoveAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;
	AActor* Owner = MeshComp->GetOwner();
	AARCharacterPlayer* Player = Cast<AARCharacterPlayer>(MeshComp->GetOwner());

    UWorld* World = MeshComp->GetWorld();
    if (!World)
        return;

    // 에디터 미리보기 또는 애님프리뷰 월드 차단
    if (World->IsPreviewWorld())
        return;

    if (!Owner)
        return;

    if (!Player)
        return;

	
	UAbilitySystemComponent* ASC = Player->GetAbilitySystemComponent();

    FGameplayTagContainer OwnedTags;
    ASC->GetOwnedGameplayTags(OwnedTags);


	if (ASC && TagToRemove.IsValid())
	{
        while (ASC->HasMatchingGameplayTag(TagToRemove))
        {
            ASC->RemoveLooseGameplayTag(TagToRemove);
        }
	}
}
