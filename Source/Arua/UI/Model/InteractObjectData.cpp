// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Model/InteractObjectData.h"

void UInteractObjectData::SetName(const FString& InName)
{
	Name = InName;
	NotifyModelDataChanged(FName("Text"));
}
