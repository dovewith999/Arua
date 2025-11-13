// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	void InitializeMenu(class UInventoryComponent* InInventory, int32 InSlotIndex);

protected:
	virtual void NativeConstruct() override;

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

protected:
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

	// 연동된 인벤토리
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UInventoryComponent> Inventory;

	// 연동된 인벤토리 슬롯 인덱스
	int32 SlotIndex;
};
