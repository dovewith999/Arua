#pragma once

#include "Arua_EnumTypes.generated.h"

/**
 * 아이템 분류 열거형
 */
UENUM(BlueprintType)
enum class EAR_ItemCategory : uint8
{
	Equipment       UMETA(DisplayName = "장비"),
	Consumable		UMETA(DisplayName = "소모품"),
	Quest			UMETA(DisplayName = "퀘스트"),
	Misc			UMETA(DisplayName = "기타")
};

/**
 * 장비 아이템을 장착할 슬롯 타입을 정의하는 열거형
 * 무기, 방어구, 장신구 등 장비 슬롯을 정의
 */
UENUM(BlueprintType)
enum class EAR_EquipmentSlotType : uint8
{
	None			UMETA(DisplayName = "없음"),
	Weapon			UMETA(DisplayName = "무기"),
	Armor			UMETA(DisplayName = "방어구"),
	Accessory		UMETA(DisplayName = "장신구"),
};
