// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Item/DA_EquipmentDefinition.h"
#include "Character/ARCharacterPlayer.h"
#include "Character/Weapon/ARWeaponBase.h"
#include "Components/Inventory/InventoryComponent.h"
#include "Actors/Items/ItemPickupActor.h"

void UDA_EquipmentDefinition::ApplyEffect_Implementation(AActor* User)
{
	if (!WeaponDataClass || !User || !User->GetWorld()) return;

	if (AARCharacterPlayer* Player = Cast<AARCharacterPlayer>(User))
	{
		FActorSpawnParameters Params;
		Params.Owner = User;
		Params.Instigator = Cast<APawn>(User);
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		// 장착할 무기 클래스의 실제 인스턴스 생성
		AARWeaponBase* SpawnedWeapon = User->GetWorld()->SpawnActor<AARWeaponBase>(
			WeaponDataClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			Params
		);

		// 무기 인스턴스가 생성되지 않았으면 종료
		if (!SpawnedWeapon) return;

		// 무기 아이템의 무기로 플레이어의 무기를 교체
		const AARWeaponBase* OriginalWeapon = Player->WeaponChange(SpawnedWeapon);

		if (UInventoryComponent* PlayerInventory = Player->GetInventoryComponent())
		{
			// 플레이어의 인벤토리에 교체되어 집어넣어지는 무기를 아이템으로 추가
			PlayerInventory->AddItem(OriginalWeapon->WeaponItemDefinition, 1);
		}
	}
}
