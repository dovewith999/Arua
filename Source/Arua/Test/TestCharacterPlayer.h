// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ARCharacterPlayer.h"
#include "AbilitySystemInterface.h"
#include "TestCharacterPlayer.generated.h"

/**
 * LockOn 기능 등 GAS 기반 기능을 테스트하기 위한 캐릭터 클래스입니다.
 * 작성자 : 임희섭
 * 작성일 : 25/11/04
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


public:
	FORCEINLINE void SetTarget(AActor* InTarget) { Target = InTarget; }
	FORCEINLINE const AActor* GetTarget() const { return Target; }

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

protected:
	// LockOn은 토글 방식으로
	void LockOnToggle(const FInputActionValue& Value);

private:
	UPROPERTY();
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LockOnAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class UGameplayAbility>> StartingAbilities;

private:
	UPROPERTY()
	TObjectPtr<AActor> Target;

};
