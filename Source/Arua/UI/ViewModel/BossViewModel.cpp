// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/BossViewModel.h"
#include "UI/Model/BossData.h"

void UBossViewModel::Initialize(class UModelBase* InModel)
{
	Model = Cast<UBossData>(InModel);

	if (Model)
	{
		Model->OnModelDataChanged.AddUObject(this, &UBossViewModel::OnModelDataChanged);
	}
}

void UBossViewModel::SetHp(float NewHp)
{
	Model->SetHp(NewHp);
	NotifyPropertyChanged(FName(TEXT("Hp")));
}

void UBossViewModel::SetMaxHp(float NewMaxHp)
{
	Model->SetMaxHp(NewMaxHp);
	NotifyPropertyChanged(FName(TEXT("MaxHp")));
}

FString UBossViewModel::GetName() const
{
	return Model->GetName();
}

void UBossViewModel::SetName(const FString& InNewName)
{
	Model->SetName(InNewName);
	NotifyPropertyChanged(FName(TEXT("Name")));
}

void UBossViewModel::TakeDamage(float InDamage)
{
	Model->TakeDamage(InDamage);
	NotifyPropertyChanged(FName(TEXT("Hp")));
}

float UBossViewModel::GetHpRatio() const
{
	return Model->GetHpRatio();
}

void UBossViewModel::OnModelDataChanged(const FName& PropertyName)
{
	NotifyPropertyChanged(PropertyName);
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, PropertyName.ToString());
}