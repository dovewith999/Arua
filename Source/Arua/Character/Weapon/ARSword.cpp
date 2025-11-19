// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Weapon/ARSword.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Character/ARCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet/ARAttributeSetBase.h"
#include "AttributeSet/PlayerAttributeSet.h"

AARSword::AARSword()
{
	ChargeEffect1 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ChargeEffect1"));
	ChargeEffect1->SetupAttachment(RootComponent);

	ChargeEffect2 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ChargeEffect2")); 
	ChargeEffect2->SetupAttachment(RootComponent);
}

void AARSword::ChargeEffect()
{
	if (!OwnerPlayer)
	{
		return;
	}

	const UPlayerAttributeSet* AttributeSet = Cast<UPlayerAttributeSet>(OwnerPlayer->GetAbilitySystemComponent()->GetAttributeSet(UPlayerAttributeSet::StaticClass()));

	if (AttributeSet->GetChargeCount() == 0)
	{
		WeaponStaticMesh->SetVisibility(true);

		//ChargeEffect1->Deactivate();
		//ChargeEffect2->Deactivate();
	}

	else if (AttributeSet->GetChargeCount() == 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Charge Count1"));

		WeaponStaticMesh->SetVisibility(false);
		ChargeEffect1->Activate();;
	}

	else if (AttributeSet->GetChargeCount() == 2)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Charge Count2"));

		ChargeEffect1->Deactivate();
		ChargeEffect2->Activate();
	}
}

void AARSword::InitVisible()
{
	WeaponStaticMesh->SetVisibility(true);

	ChargeEffect1->Deactivate();
	ChargeEffect2->Deactivate();
}

void AARSword::InitializeFromData()
{
	Super::InitializeFromData();
}
