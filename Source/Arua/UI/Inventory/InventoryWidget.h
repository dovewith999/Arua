// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AruaTypes/Arua_EnumTypes.h"
#include "InventoryWidget.generated.h"

// 키 입력 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRequestInputInInventoryWidget);

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
	void InitializeInventory(class UInventoryComponent* InInventory);

	// 현재 카테고리 설정 함수, 그리드 리셋
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetCategory(EAR_ItemCategory NewCategory);

	// 같은 카테고리의 다음 페이지로 이동하는 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void NextPage();

	// 같은 카테고리의 이전 페이지로 이동하는 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void PrevPage();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// 키 다운 콜백 함수
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	// 마우스 버튼 콜백 함수
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;


	// 인벤토리 변경 시 호출되는 함수
	UFUNCTION()
	void OnInventoryUpdated();

	// 그리드(슬롯 위젯) 재생성 함수
	void RefreshGrid();

public:
	// 키 입력 델리게이트 변수
	UPROPERTY(BlueprintAssignable)
	FOnRequestInputInInventoryWidget OnRequestInputInInventoryWidget;

protected:
	// 아이템 슬롯을 배치할 그리드
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUniformGridPanel> SlotGrid;

	// 다음 페이지 전환 버튼
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> NextPageButton;

	// 이전 페이지 전환 버튼
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> PrevPageButton;

	// 슬롯 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UInventorySlotWidget> SlotWidgetClass;

	// 현재 선택된 카테고리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	EAR_ItemCategory CurrentCategory = EAR_ItemCategory::Equipment;

	// 현재 페이지 인덱스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	int32 CurrentPageIndex = 0;

	// 현재 연결된 인벤토리 컴포넌트
	UPROPERTY()
	TObjectPtr<class UInventoryComponent> Inventory;
};
