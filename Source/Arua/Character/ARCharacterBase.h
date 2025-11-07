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

	// 컴포넌트 초기화가 끝나면 호출되는 이벤트 함수. 
	virtual void PostInitializeComponents() override;

protected:
	// 죽음 설정 함수.
	// 죽음 상태 설정
	virtual void SetDead();

	void PlayDeadAnimation();

	// ASC
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS)
	TObjectPtr<class UARAttributeSetBase> AttributeSet;

};
