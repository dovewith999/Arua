// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "ARCharacterBase.generated.h"

UCLASS()
class ARUA_API AARCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARCharacterBase();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	class UARAttributeSetBase* GetAttributeSet() const { return AttributeSet; }
	FORCEINLINE class UAnimMontage* GetDeadMontage() const { return DeadMontage; }

	// 컴포넌트 초기화가 끝나면 호출되는 이벤트 함수. 
	virtual void PostInitializeComponents() override;

public:
	// 죽음 섹션
	virtual void SetDead();

protected:

	virtual void PlayDeadAnimation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	// ASC
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS)
	TObjectPtr<class UARAttributeSetBase> AttributeSet;

};
