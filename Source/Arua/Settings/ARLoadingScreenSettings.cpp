// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/ARLoadingScreenSettings.h"
#include "Blueprint/UserWidget.h"

TSubclassOf<UUserWidget> UARLoadingScreenSettings::GetLoadingScreenWidgetClassCheck() const
{

   TSubclassOf<UUserWidget> LoadedLoadingScreenWidget =  SoftLoadingScreenWidgetClass.LoadSynchronous();
    
    return LoadedLoadingScreenWidget;
}
