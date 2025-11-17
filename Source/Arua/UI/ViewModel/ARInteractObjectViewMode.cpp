// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/ARInteractObjectViewMode.h"
#include "UI/Model/InteractObjectData.h"

void UARInteractObjectViewMode::Initialize(UModelBase* InModel)
{
	Model = Cast<UInteractObjectData>(InModel);
	if (Model)
	{
		Model->OnModelDataChanged.AddUObject(this, &UARInteractObjectViewMode::OnModelDataChanged);
	}
}

void UARInteractObjectViewMode::OnModelDataChanged(const FName& PropertyName)
{
	NotifyPropertyChanged(PropertyName);
}