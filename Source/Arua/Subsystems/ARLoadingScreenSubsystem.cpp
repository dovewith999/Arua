// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLoadingScreenSubsystem.h"
#include "PreLoadScreenManager.h"
#include "Settings/ARLoadingScreenSettings.h"
#include "Blueprint/UserWidget.h"
#include "Interface/ARLoadingScreenInterface.h"

bool UARLoadingScreenSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}

	return false;
}

void UARLoadingScreenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ThisClass::OnMapPreLoaded);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnMapPostLoaded);
}

void UARLoadingScreenSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PreLoadMapWithContext.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

UWorld* UARLoadingScreenSubsystem::GetTickableGameObjectWorld() const
{
	if (UGameInstance* OwningGameInstance = GetGameInstance())
	{
		return OwningGameInstance->GetWorld();
	}
	return nullptr;
}

void UARLoadingScreenSubsystem::Tick(float DeltaTime)
{
	TryUpdateLoadingScreen();
}

ETickableTickType UARLoadingScreenSubsystem::GetTickableTickType() const
{
	if (IsTemplate())
	{
		return ETickableTickType::Never;
	}

	return ETickableTickType::Conditional;
}

bool UARLoadingScreenSubsystem::IsTickable() const
{
	return GetGameInstance() && GetGameInstance()->GetGameViewportClient();
}

TStatId UARLoadingScreenSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UARLoadingScreenSubsystem, STATGROUP_Tickables);
}

void UARLoadingScreenSubsystem::OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName)
{
	if (WorldContext.OwningGameInstance != GetGameInstance())
	{
		return;
	}

	SetTickableTickType(ETickableTickType::Conditional);

	bIsCurrentlyLoadingMap = true;

	TryUpdateLoadingScreen();
}

void UARLoadingScreenSubsystem::OnMapPostLoaded(UWorld* LoadedWorld)
{
	if (LoadedWorld && LoadedWorld->GetGameInstance() == GetGameInstance())
	{
		bIsCurrentlyLoadingMap = false;
	}
}

void UARLoadingScreenSubsystem::TryUpdateLoadingScreen()
{
	if (IsPreLoadScreenActive())
	{
		return;
	}

	// 현재 로딩 스크린을 띄울 수 있는지 검사
	if (ShouldShowLoadingScreen())
	{
		// 로딩 스크린 띄우기
		TryDisplayLoadingScreenIfNone();
		 
		// 로딩 이유 표시하는 UI에 글 표시하기 
		OnLoadingReasonUpdated.Broadcast(CurrentLoadingReason);
	}

	else
	{
		// 로딩 스크린 비활성화
		TryRemoveLoadingScreen();
		HoldLoadingScreenStartUpTime = -1.f;

		// 로딩 완료 Notify 실행 
		
		// 틱 비활성화
		SetTickableTickType(ETickableTickType::Never);
	}
}

bool UARLoadingScreenSubsystem::IsPreLoadScreenActive() const
{
	if (FPreLoadScreenManager* PreLoadScreenManager = FPreLoadScreenManager::Get())
	{
		return PreLoadScreenManager->HasValidActivePreLoadScreen();
	}

	return false;
}

bool UARLoadingScreenSubsystem::ShouldShowLoadingScreen()
{
	const UARLoadingScreenSettings* LoadingScreenSettings = GetDefault<UARLoadingScreenSettings>();

	if (GIsEditor && !LoadingScreenSettings->bShouldLoadingScreenInEditor)
	{
		return false;
	}

	if (CheckTheNeedToShowLoadingScreen())
	{
		GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = true;

		return true;
	}

	CurrentLoadingReason = TEXT("Waiting for Texture Streaming");

	GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = false;

	const float CurrentTime = FPlatformTime::Seconds();

	if (HoldLoadingScreenStartUpTime < 0.f)
	{
		HoldLoadingScreenStartUpTime = CurrentTime;
	}

	const float ElapsedTime = CurrentTime - HoldLoadingScreenStartUpTime;

	if (ElapsedTime < LoadingScreenSettings->HoldLoadingScreenExtraSceonds)
	{
		return true;
	}

	return false;
}

bool UARLoadingScreenSubsystem::CheckTheNeedToShowLoadingScreen()
{
	// 1. 맵 로딩 중인지 확인
	if (bIsCurrentlyLoadingMap)
	{
		CurrentLoadingReason = TEXT("Loading Level");
		return true;
	}

	// 2. 월드가 생성됐는지 확인
	UWorld* OwningWorld = GetGameInstance()->GetWorld();

	if (!OwningWorld)
	{
		CurrentLoadingReason = TEXT("Initializing Level");

		return true;
	}

	// 3. BegunPlay 호출 여부 확인
	if (!OwningWorld->HasBegunPlay())
	{
		CurrentLoadingReason = TEXT("World hasn't Begunplay yet");

		return true;
	}

	// 4. PlayerController 생성 여부 확인
	if (!OwningWorld->GetFirstPlayerController())
	{
		CurrentLoadingReason = TEXT("Player Controller Is Not World Yet");

		return true;
	}

	return false;
}

void UARLoadingScreenSubsystem::TryDisplayLoadingScreenIfNone()
{
	// 이미 활성화된 로딩 화면이 있는지 확인
	if (CachedCreateLoadingScreenWidget)
	{
		return;
	}

	const UARLoadingScreenSettings* LoadingScreenSettings = GetDefault<UARLoadingScreenSettings>();

	TSubclassOf<UUserWidget> LoadedWidgetClass = LoadingScreenSettings->GetLoadingScreenWidgetClassCheck();

	UUserWidget* CreateWidget = UUserWidget::CreateWidgetInstance(*GetGameInstance(), LoadedWidgetClass, NAME_None);

	CachedCreateLoadingScreenWidget = CreateWidget->TakeWidget();

	GetGameInstance()->GetGameViewportClient()->AddViewportWidgetContent(
		CachedCreateLoadingScreenWidget.ToSharedRef(),
		1000 // 오더
	);

	NotifyLoadingScreenVisiblilityChanged(true);
}

void UARLoadingScreenSubsystem::TryRemoveLoadingScreen()
{
	if (!CachedCreateLoadingScreenWidget)
	{
		return;
	}

	GetGameInstance()->GetGameViewportClient()->RemoveViewportWidgetContent(CachedCreateLoadingScreenWidget.ToSharedRef());

	CachedCreateLoadingScreenWidget.Reset();

	NotifyLoadingScreenVisiblilityChanged(false);
}

void UARLoadingScreenSubsystem::NotifyLoadingScreenVisiblilityChanged(bool bIsVisible)
{
	for (ULocalPlayer* ExistingLocalPlayer : GetGameInstance()->GetLocalPlayers())
	{
		if (!ExistingLocalPlayer)
		{
			continue;
		}

		if(APlayerController* PC = ExistingLocalPlayer->GetPlayerController(GetGameInstance()->GetWorld()))
		{
			if (PC->Implements<UARLoadingScreenInterface>())
			{
				if (bIsVisible)
				{
					IARLoadingScreenInterface::Execute_OnLoadingScreenActivated(PC);
				}
				else
				{
					IARLoadingScreenInterface::Execute_OnLoadingScreenDeactivated(PC);

				}
			}

			if (APawn* OwningPawn = PC->GetPawn())
			{
				if (OwningPawn->Implements<UARLoadingScreenInterface>())
				{
					if (bIsVisible)
					{
						IARLoadingScreenInterface::Execute_OnLoadingScreenActivated(PC);
					}
					else
					{
						IARLoadingScreenInterface::Execute_OnLoadingScreenDeactivated(PC);

					}
				}
			}
		}

		
	}
}
