// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "ARWidgetComponent.generated.h"

/**
 * 3D 위젯을 위한 위젯 컴포넌트
 * 작성자 : 임희섭
 * 작성일 : 25/11/05
 */
UCLASS()
class ARUA_API UARWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

protected:
	virtual void InitWidget() override;
	
};
