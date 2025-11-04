// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ARCharacterPlayer.h"
#include "TestCharacterPlayer.generated.h"

class UPlayerViewModel;
/**
 * 
 */
UCLASS()
class ARUA_API ATestCharacterPlayer : public AARCharacterPlayer
{
	GENERATED_BODY()

public:
	ATestCharacterPlayer();

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewModel", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerViewModel> VM;

	UPROPERTY(VisibleAnywhere, Category = "HUD")
	TSubclassOf<class UPlayerHUDView> HUDClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPlayerHUDView> HUD;
};
