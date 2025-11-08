// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncActions/AsyncAction_PushSoftWidget.h"
#include "UI/View/CommonActivatableViewBase.h"
#include "Subsystems/UISubsystem.h"

UAsyncAction_PushSoftWidget* UAsyncAction_PushSoftWidget::PushSoftWidget(const UObject* WorldContextObject, APlayerController* OwningPlayerController, TSoftClassPtr<class UCommonActivatableViewBase> InSoftWidgetClass, UPARAM(meta = (Categories = "WidgetStack")) FGameplayTag InWidgetStackTag, bool bFocusOnNewlyPushedWidget)
{
	ensureAlwaysMsgf(!InSoftWidgetClass.IsNull(), TEXT("PushSoftWidget was passed a null soft widget class"));

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Green, TEXT("GEngine is valid"));

		// World를 받아오는 주체를 WorldContextObject에서 OwningPlayerController로 변경
		// WorldContextObject로 하면 null이 뜸
		if (UWorld* World = GEngine->GetWorldFromContextObject(OwningPlayerController, EGetWorldErrorMode::LogAndReturnNull))
		{
			GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Blue, TEXT("World is valid"));

			UAsyncAction_PushSoftWidget* Node = NewObject<UAsyncAction_PushSoftWidget>();
			Node->CachedOwningWorld = World;
			Node->CachedOwningPC = OwningPlayerController;
			Node->CachedSoftWidgetClass = InSoftWidgetClass;
			Node->CachedWidgetStackTag = InWidgetStackTag;
			Node->bCachedFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;

			Node->RegisterWithGameInstance(World);

			return Node;
		}
	}

	return nullptr;
}

void UAsyncAction_PushSoftWidget::Activate()
{
	Super::Activate();

	GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Blue, TEXT("Activate"));

	// 비동기 로직 처리
	UUISubsystem* UISubsystem = UUISubsystem::Get(CachedOwningWorld.Get());

	UISubsystem->PushSoftWidgetToSackAsync(CachedWidgetStackTag, CachedSoftWidgetClass, 
		[this](EAsyncPushWidgetState InPushState, TObjectPtr<UCommonActivatableViewBase> PushedWidget)
		{
			switch (InPushState)
			{
			case EAsyncPushWidgetState::OnCreatedBeforePush:

				PushedWidget->SetOwningPlayer(CachedOwningPC.Get());
				OnWidgetCreatedBeforePush.Broadcast(PushedWidget);

				break;
			case EAsyncPushWidgetState::AfterPush:
				AfterPush.Broadcast(PushedWidget);

				if (bCachedFocusOnNewlyPushedWidget)
				{
					if (UWidget* WidgetToFocus = PushedWidget->GetDesiredFocusTarget())
					{
						WidgetToFocus->SetFocus();
					}
				}

				SetReadyToDestroy();

				break;
			default:
				break;
			}
		});
}
