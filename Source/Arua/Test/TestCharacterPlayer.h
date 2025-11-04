// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ARCharacterPlayer.h"
#include "AbilitySystemInterface.h"
#include "TestCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API ATestCharacterPlayer : public AARCharacterPlayer, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ATestCharacterPlayer();

public:
	// Inherited via IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

protected:
	// Started: 마우스 휠 버튼을 누를 때
	void LockOnPressed(const FInputActionValue& Value);

	// Completed: 마우스 휠 버튼을 뗄 때
	void LockOnReleased(const FInputActionValue& Value);

private:
	UPROPERTY();
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LockOnAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class UGameplayAbility>> StartingAbilities;

};
