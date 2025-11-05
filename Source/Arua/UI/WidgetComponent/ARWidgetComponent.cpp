// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetComponent/ARWidgetComponent.h"
#include "UI/View/ViewBase.h"

void UARWidgetComponent::InitWidget()
{
	Super::InitWidget();

	// 상위에서 InitWidget 함수가 호출된 이후에는 위젯 인스턴스가 생성됐다는 것을 보장할 수 있다.
	if (UViewBase* UserWidget = Cast<UViewBase>(GetWidget()))
	{
		UserWidget->SetOwningActor(GetOwner());
	}
}
