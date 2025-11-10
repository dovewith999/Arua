#pragma once

// 아이템 분류 열거형
UENUM(BlueprintType)
enum class EAR_ItemCategory : uint8
{
    Armor       UMETA(DisplayName = "장비"),
    Consumable  UMETA(DisplayName = "소모품"),
    Quest       UMETA(DisplayName = "퀘스트"),
    Misc        UMETA(DisplayName = "기타")
};