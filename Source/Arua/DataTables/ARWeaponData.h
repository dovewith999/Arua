
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

	// 무기 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	EWeaponType WeaponType;


	// 무기 스태틱 메시
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data)
	TSoftObjectPtr<UStaticMesh> WeaponMeshAsset;

	// Idle 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TSoftObjectPtr<UAnimSequence> IdleAnimation;

	// 락온이 되지 않았을 때 걷기 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TSoftObjectPtr<UAnimSequence> NotLockOnWalkAnimation;

	// 락온이 되었을 때 걷기 모션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TSoftObjectPtr<UBlendSpace> LockOnWalkAnimation;
	
	// 공격 애님 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TSoftObjectPtr<UAnimMontage> AttackMontage;

	// 스페이스바 몽타주 - 구르기나 슬라이드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TSoftObjectPtr<UAnimMontage> SpaceBarAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float AttackPower;

};