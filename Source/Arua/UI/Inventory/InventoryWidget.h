// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 인벤토리 UI를 표시하는 위젯
 */
UCLASS()
class ARUA_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 인벤토리 위젯 초기화 함수
	/** 인벤토리 설정 및 이벤트 바인딩 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InitializeInventory(UInventoryComponent* InInventory);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// 인벤토리 변경 시 호출되는 함수
	UFUNCTION()
	void OnInventoryUpdated();

	// 스크롤 박스를 비우고 슬롯 위젯을 재생성하는 함수
	void RefreshInventory();

protected:
	// 인벤토리 슬롯을 표시하는 ScrollBox. 블루프린트에서 BindWidget으로 지정함
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScrollBox> InventoryScrollBox;

	// 슬롯 위젯 클래스. 블루프린트에서 지정함
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TSubclassOf<class UInventorySlotWidget> SlotWidgetClass;

	// 현재 연결된 인벤토리 컴포넌트
	UPROPERTY()
	TObjectPtr<UInventoryComponent> Inventory;

	// 한 페이지에 표시할 슬롯 수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 SlotsPerPage = 20;
};
