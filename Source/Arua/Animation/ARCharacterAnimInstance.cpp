// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ARCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/ARCharacterPlayer.h"
#include "AbilitySystemComponent.h"

UARCharacterAnimInstance::UARCharacterAnimInstance()
{
	MovingThreshould = 3.0f;
}

void UARCharacterAnimInstance::PlayActionMontage(const FGameplayTag& WeaponTag, const FGameplayTag& ActionTag)
{
	UAnimMontage* MontageToPlay = FindMontageInternal(CurrentWeaponTag, ActionTag);

	if (MontageToPlay)
	{
		UE_LOG(LogTemp, Log, TEXT("PlayActionMontage"));
		Montage_Play(MontageToPlay);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No montage found for Weapon(%s) / Action(%s)"),
			*CurrentWeaponTag.ToString(), *ActionTag.ToString());
	}
}

void UARCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}

	StateMachineIndex = 1;
	bASCInitialized = false;

	//if (!MontageDataAsset) return;

	//// 공용 몽타주 초기화
	//CommonMontageTable = MontageDataAsset->CommonMontages;

	//// Weapon + Action → Montage 매핑
	//for (const FWeaponActionMontage& Entry : MontageDataAsset->WeaponActionMontages)
	//{
	//	if (!WeaponActionMontageTable.Contains(Entry.WeaponTag))
	//	{
	//		WeaponActionMontageTable.Add(Entry.WeaponTag, TMap<FGameplayTag, UAnimMontage*>());
	//	}
	//	WeaponActionMontageTable[Entry.WeaponTag].Add(Entry.ActionTag, Entry.Montage);
	//}
	
}

void UARCharacterAnimInstance::NativeUpdateAnimation(float DeltaTimes)
{
	Super::NativeUpdateAnimation(DeltaTimes);
	AARCharacterPlayer* Player = Cast<AARCharacterPlayer>(Owner);

	if (!bASCInitialized)
	{
		if (!Player)
		{
			return;
		}

		UAbilitySystemComponent* PA = Player->GetAbilitySystemComponent();
		if (PA)
		{
			// 이벤트 등록 등 초기화
			bASCInitialized = true;
			PA->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("Character.Weapon"))
				.AddUObject(this, &UARCharacterAnimInstance::OnGameplayTagChanged);

			Player->EquipStartWeapon();
		}
	}

	if (Movement)
	{
		Velocity = Movement->Velocity;
		WalkSpeed = Velocity.Size2D();
		bIsIdle = WalkSpeed < MovingThreshould;
		bIsWalk = Player->GetWalkState();
		bIsRun = Player->GetRunState();

		// 이동 방향
		FVector VelocityDirection = Velocity.GetSafeNormal();

		// 캐릭터의 Forward 벡터
		FVector Forward = Owner->GetActorForwardVector();

		// 각도 계산 (-180 ~ 180)
		Angle = FMath::RadiansToDegrees(FMath::Atan2(
			FVector::CrossProduct(Forward, VelocityDirection).Z,
			FVector::DotProduct(Forward, VelocityDirection)
		));
	}

	
}

void UARCharacterAnimInstance::OnGameplayTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag("Character.Weapon")), EGameplayTagMatchType::IncludeParentTags)
	{
		CurrentWeaponTag = Cast<AARCharacterPlayer>(Owner)->GetWeaponTag();
		StateMachineIndex = GetWeaponLayerIndex();

	}
}

int32 UARCharacterAnimInstance::GetWeaponLayerIndex() const
{

	AARCharacterPlayer* Player = Cast<AARCharacterPlayer>(Owner);
	const FGameplayTagContainer& Tags = Player->GetAbilitySystemComponent()->GetOwnedGameplayTags();

	if (Tags.HasTagExact(FGameplayTag::RequestGameplayTag("Character.Weapon.None")))
	{
		return 0;
	}
		

	if (Tags.HasTagExact(FGameplayTag::RequestGameplayTag("Character.Weapon.Sword")))
	{
		return 1;
	}

	if (Tags.HasTagExact(FGameplayTag::RequestGameplayTag("Character.Weapon.Katana")))
	{
		return 2;
	}

	return 0;
}

UAnimMontage* UARCharacterAnimInstance::FindMontageInternal(FGameplayTag WeaponTag, FGameplayTag ActionTag) const
{
	if (!MontageData)
		return nullptr;

	// 1) 무기별 몽타주 우선 검색
	if (UAnimMontage* WeaponMontage = MontageData->FindWeaponMontage(WeaponTag, ActionTag))
	{
			return WeaponMontage;
	}
		

	// 2) 없으면 공통(ActionTag) 몽타주 사용
	if (UAnimMontage* CommonMontage = MontageData->FindCommonMontage(ActionTag))
		return CommonMontage;

	return nullptr;
}


