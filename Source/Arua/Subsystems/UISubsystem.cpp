// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/UISubsystem.h"
#include "UI/View/Common/PrimaryLayoutView.h"
#include "Engine/AssetManager.h"
#include "GameplayTagContainer.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "UI/View/CommonActivatableViewBase.h"

UUISubsystem* UUISubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);

		return UGameInstance::GetSubsystem<UUISubsystem>(World->GetGameInstance());
	}

	return nullptr;
}

bool UUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}

	return false;
}

void UUISubsystem::RegisterCreatedPrimaryLayoutWidget(UPrimaryLayoutView* InCreatedWidget)
{
	ensureAlways(InCreatedWidget);

	CreatedPrimaryLayout = InCreatedWidget;
}

void UUISubsystem::PushSoftWidgetToSackAsync(const FGameplayTag& InWidgetStackTag, TSoftClassPtr<UCommonActivatableViewBase> InSoftWidgetClass, TFunction<void(EAsyncPushWidgetState, TObjectPtr<UCommonActivatableViewBase>)> AsyncPushStateCallback)
{
	ensureAlways(!InSoftWidgetClass.IsNull());

	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		InSoftWidgetClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[InSoftWidgetClass, this, InWidgetStackTag, AsyncPushStateCallback]()
			{
				if (UClass* LoadWidgetClass = InSoftWidgetClass.Get())
				{
					UCommonActivatableWidgetContainerBase* FoundWidgetStack = CreatedPrimaryLayout->FindWidgetStackByTag(InWidgetStackTag);

					UCommonActivatableViewBase* CreatedWidget = FoundWidgetStack->AddWidget<UCommonActivatableViewBase>(
						LoadWidgetClass,
						[AsyncPushStateCallback](UCommonActivatableViewBase& CreatedWidgetInstance)
						{
							AsyncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);
						});

						AsyncPushStateCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
				}
			}));
}
