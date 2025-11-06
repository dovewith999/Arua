// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AruaPlayerController.generated.h"

class UPlayerViewModel;
class UPlayerHUDView;
/**
 * Player에게 빙의할 Controller 클래스 입니다.
 * 작성자 : 임희섭
 * 작성일 : 25/11/05
 */
UCLASS()
class ARUA_API AAruaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAruaPlayerController();

public:
	void SetTargetBoss(class AARMonsterBase* InTargetMonster);

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;

private:
	UPROPERTY()
	TObjectPtr<UPlayerViewModel> ViewModel;

	UPROPERTY()
	TObjectPtr<UPlayerHUDView> HUD;

	UPROPERTY()
	TSubclassOf<UPlayerHUDView> HUDClass;
};
