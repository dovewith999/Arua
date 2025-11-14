// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemQuantityPopupWidget.generated.h"

// 아이템 팝업 시 수행할 액션 열거형
UENUM(BlueprintType)
enum class EItemPopUpAction : uint8
{
	BundleUse	UMETA(DisplayName = "묶음 사용"),
	Split		UMETA(DisplayName = "나누기"),
	Remove		UMETA(DisplayName = "제거")
};

/**
 * 여러 개 아이템을 사용할 때, 나눌 때, 제거할 때 수량을 선택하는 팝업 위젯
 */
UCLASS()
class ARUA_API UItemQuantityPopupWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 팝업 초기화 함수
	UFUNCTION(BlueprintCallable, Category = "PopUp")
	void InitializePopUp(class UInventoryComponent* InInventory, int32 InSlotIndex, EItemPopUpAction InAction);

protected:
	virtual void NativeConstruct() override;

	// 확인 버튼 이벤트
	UFUNCTION()
	void OnConfirmCliked();

	// 취소 버튼 이벤트
	UFUNCTION()
	void OnCancelCliked();

protected:
	// 팝업 제목 텍스트 (ex. 아이템 사용, 아이템 나누기 등)
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> TitleText;

	// 아이템 이름
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemName;

	// 아이템 아이콘 이미지
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UImage> ItemIconImage;

	// 현재 수량/최대 수량 표시 텍스트
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> QuantityLabel;

	// 수량 선택 스핀박스
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class USpinBox> QuantitySpinBox;

	// 확인 버튼
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UButton> ConfirmButton;

	// 취소 버튼
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UButton> CancelButton;

private:
	// 연동된 인벤토리
	UPROPERTY()
	TObjectPtr<class UInventoryComponent> Inventory;

	// 연동된 인벤토리 슬롯 인덱스
	int32 SlotIndex;

	// 수행할 팝업 액션
	EItemPopUpAction Action;
};
