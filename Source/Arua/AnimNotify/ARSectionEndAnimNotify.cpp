// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/ARSectionEndAnimNotify.h"
#include "Character/ARCharacterPlayer.h"

void UARSectionEndAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp) return;

    AARCharacterPlayer* Char = Cast<AARCharacterPlayer>(MeshComp->GetOwner());
    if (Char)
    {
        Char->MontageEnded();
        UE_LOG(LogTemp, Log, TEXT("Notify Success"));
    }
}
