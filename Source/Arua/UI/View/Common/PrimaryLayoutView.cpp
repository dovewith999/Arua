// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryLayoutView.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

UCommonActivatableWidgetContainerBase* UPrimaryLayoutView::FindWidgetStackByTag(const FGameplayTag& InStackTag)
{
	ensureAlways(RegisteredWidgetStackMap.Contains(InStackTag));

	return RegisteredWidgetStackMap.FindRef(InStackTag);
}

void UPrimaryLayoutView::RegisterWidgetStack(UPARAM(meta = (Categories = "WidgetStack")) FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack)
{
	if (!IsDesignTime())
	{
		if (!RegisteredWidgetStackMap.Contains(InStackTag))
		{
			RegisteredWidgetStackMap.Add(InStackTag, InStack);

			UE_LOG(LogTemp, Log, TEXT("Widget Stack Register : %s"), *InStackTag.ToString());
		}
	}
}
