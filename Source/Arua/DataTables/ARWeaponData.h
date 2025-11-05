
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ARWeaponData.generated.h"

USTRUCT(BlueprintType)
struct FARWeaponData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FName WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FName SkeletalMeshPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FName IdlePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FName WalkPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FName RunPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FName AttackMontagePath;
};