// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AruaTypes/Arua_EnumTypes.h"
#include "ItemContextMenuWidget.generated.h"

/**
 * 우클릭 시 표시되는 아이템 컨텍스트 메뉴 위젯
 */
UCLASS()
class ARUA_API UItemContextMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 콘텍스트 메뉴 초기화 함수
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void InitializeMenu(class UInventoryComponent* InInventory, EAR_ItemCategory InCategory, int32 InSlotIndex);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

	// [사용] 이벤트
	UFUNCTION()
	void OnUseClicked();

	// [묶음 사용] 이벤트
	UFUNCTION()
	void OnBundleUseClicked();

	// [나누기] 이벤트
	UFUNCTION()
	void OnSplitClicked();

	// [제거] 이벤트
	UFUNCTION()
	void OnRemoveClicked();

	// [취소] 이벤트
	UFUNCTION()
	void OnCancelClicked();

	// 아이템 수량 선택 팝업 위젯 설정 함수
	void SetUpItemQuantityPopupWidget(class UItemQuantityPopupWidget* PopUpWidget, EItemPopUpAction InAction);

protected:
	// 아이템 이름
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemName;

	// [사용] 버튼 위젯
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UButton> OnUseButton;

	// [묶음 사용] 버튼 위젯
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UButton> OnBundleUseButton;

	// [나누기] 버튼 위젯
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UButton> OnSplitButton;

	// [제거] 버튼 위젯
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UButton> OnRemoveButton;

	// [취소] 버튼 위젯
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UButton> OnCancleButton;

	// 아이템 수량 선택 팝업 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UItemQuantityPopupWidget> ItemQuantityPopupWidgetClass;

	// 현재 연결된 인벤토리 컴포넌트
	UPROPERTY()
	TObjectPtr<class UInventoryComponent> Inventory;

	// 슬롯 카테고리
	EAR_ItemCategory Category;

	// 슬롯 인덱스
	int32 SlotIndex;
};
