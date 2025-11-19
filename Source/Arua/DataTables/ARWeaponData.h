
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ARWeaponData.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None UMETA(DisplayName = "None"),
	Sword UMETA(DisplayName = "Sword")
};


USTRUCT(BlueprintType)
struct FARWeaponData : public FTableRowBase
{
	GENERATED_BODY()

public:

	// 무기 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FName WeaponName;

	// UI 표시용 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data)
	FText DisplayName;

	// 무기 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FName WeaponTag;

	// 무기 스태틱 메시
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data)
	TSoftObjectPtr<UStaticMesh> WeaponMeshAsset;

	/*UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Data)
	TSoftObjectPtr<class UAnimMontage> WeaponEquipMontageData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TSoftObjectPtr<class UAnimInstance> AnimInstanceByWeaponData;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float AttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	FName Socket;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data)
	TSoftObjectPtr<class UARComboActionData> ComboActionData;

};