// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ARHitReactableInterface.generated.h"

/*
* 피격 반응이 가능한 액터들이 구현해야 하는 인터페이스
* 작성자 : 임희섭
* 작성일 : 25/11/18
*/

#define PURE = 0

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UARHitReactableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARUA_API IARHitReactableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HitReact")
	void OnHitByAttack(const FHitResult& HitResult, AActor* InInstigator);
};
