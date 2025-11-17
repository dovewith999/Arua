// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/ARInteractObjectViewMode.h"
#include "UI/Model/InteractObjectData.h"

void UARInteractObjectViewModel::Initialize(UModelBase* InModel)
{
	Model = Cast<UInteractObjectData>(InModel);
	if (Model)
	{
		Model->OnModelDataChanged.AddUObject(this, &UARInteractObjectViewModel::OnModelDataChanged);
	}
}

void UARInteractObjectViewModel::OnModelDataChanged(const FName& PropertyName)
{
	NotifyPropertyChanged(PropertyName);
}

FString UARInteractObjectViewModel::GetName() const
{
	if (Model)
	{
		return Model->GetName();
	}

	return FString("");
}