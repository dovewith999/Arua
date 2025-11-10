// Fill out your copyright notice in the Description page of Project Settings.


#include "Util/ARWidgetFunctionLibrary.h"
#include "UI/View/CommonActivatableViewBase.h"
#include "Settings/ARDeveloperSettings.h"

TSoftClassPtr<class UCommonActivatableViewBase> UARWidgetFunctionLibrary::GetAruaWidgetClassByTag(UPARAM(meta = (Categories = "Widget")) FGameplayTag InWidgetTag)
{
	const UARDeveloperSettings* DeveloperSetting = GetDefault<UARDeveloperSettings>();

	if (DeveloperSetting->AruaWidgetMap.Contains(InWidgetTag))
	{
		return DeveloperSetting->AruaWidgetMap.FindRef(InWidgetTag);
	}

	return TSoftClassPtr<class UCommonActivatableViewBase>();
}
