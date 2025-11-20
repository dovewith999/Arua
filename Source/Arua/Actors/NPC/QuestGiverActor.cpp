// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/NPC/QuestGiverActor.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/Quest/QuestComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/Dialog/DialogWidget.h"
#include "UI/Quest/QuestSelectWidget.h"
#include "Components/Interact/ARInteractComponent.h"

#include "Kismet/KismetMathLibrary.h"

static const FString QuestGiverActorContext(TEXT("QuestGiver_OnAccept"));
TMap<TWeakObjectPtr<APlayerController>, TArray<FWidgetVisibilityRecord>> AQuestGiverActor::CachedWidgetStates;

AQuestGiverActor::AQuestGiverActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// 상호작용 콜리전 박스 CDO 생성
	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetBoxExtent(FVector(100, 100, 100));
	RootComponent = InteractionVolume;

	// UI 카메라 컴포넌트 CDO 생성
	UICameraActor = CreateDefaultSubobject<UCameraComponent>(TEXT("UICameraActor"));
	UICameraActor->SetupAttachment(RootComponent);

	// NPC 위젯 컴포넌트 COD 생성
	NPCWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("NPCWidgetComponent"));
	NPCWidgetComponent->SetupAttachment(RootComponent);
	NPCWidgetComponent->SetCullDistance(5000.f);

	// InterctComponent CDO 생성
	InteractionComponent = CreateDefaultSubobject<UARInteractComponent>(TEXT("InteractComponent"));
	//InteractionComponent->SetInteractCollision(InteractionVolume, FString(TEXT("퀘스트 판")));
}

void AQuestGiverActor::Interact(APawn* InInteractor)
{
	// 플레이어 컨트롤러 가져오기 및 저장
	if (!PC)
	{
		PC = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}

	if (!PC || !InInteractor || bInteracting || !UICameraActor)
	{
		return;
	}

	// 상호작용 플레이어 초기화
	InteractorPawn = InInteractor;

	// #1: 플레이어 카메라 -> UI 카메라로 전환
	OriginalViewTarget = PC->GetViewTarget();
	PC->SetViewTargetWithBlend(this, ViewTargetBlendTime, EViewTargetBlendFunction::VTBlend_Cubic);

	// #2: UI 상호작용 모드 설정 (입력 등)
	ApplyUIInteractionMode();

	// #3: Dialog 위젯 생성 및 표시
	if (DialogWidgetClass && !DialogWidgetInstance)
	{
		DialogWidgetInstance = CreateWidget<UDialogWidget>(PC, DialogWidgetClass);
		if (DialogWidgetInstance)
		{
			// 다이얼로그 위젯 그리기
			DialogWidgetInstance->AddToViewport();

			// 다이얼로그 키 입력 이벤트와 상호작용 종료 함수 바인딩
			DialogWidgetInstance->OnRequestInput.AddDynamic(this, &AQuestGiverActor::UnInteract);
		}
	}

	// #4: 퀘스트 선택 위젯 생성 및 표시
	if (QuestSelectWidgetClass && !QuestSelectWidgetInstance)
	{
		QuestSelectWidgetInstance = CreateWidget<UQuestSelectWidget>(PC, QuestSelectWidgetClass);
		if (QuestSelectWidgetInstance)
		{
			// 퀘스트 리스트 초기화
			QuestSelectWidgetInstance->InitQuestList(InInteractor, ProvidedQuestIDs, QuestDataTables);

			// 퀘스트 선택 위젯 그리기
			QuestSelectWidgetInstance->AddToViewport();

			// 퀘스트 수락/보상 수령 바인딩
			QuestSelectWidgetInstance->OnQuestAccepted.AddDynamic(this, &AQuestGiverActor::HandleQuestAccepted);
			QuestSelectWidgetInstance->OnQuestTurnInRequested.AddDynamic(this, &AQuestGiverActor::HandleQuestTurnIn);
		}
	}

	// #5: 상호작용 상태 플래그 활성화
	GetWorld()->GetTimerManager().SetTimer(ViewTargetBlendTimer,
		FTimerDelegate::CreateWeakLambda(this, [this]()
			{
				bInteracting = true;
			}),
		ViewTargetBlendTime,
		false
	);
}

void AQuestGiverActor::PlayInteraction(APawn* InInteractor)
{
	Interact(InInteractor);
}

void AQuestGiverActor::UnInteract()
{
	if (!PC || !bInteracting || !UICameraActor) return;

	// #1: UI 카메라 -> 플레이어 카메라로 전환
	PC->SetViewTargetWithBlend(OriginalViewTarget, ViewTargetBlendTime, EViewTargetBlendFunction::VTBlend_Cubic);

	// #2: Dialog 위젯 정리
	if (DialogWidgetInstance)
	{
		DialogWidgetInstance->RemoveFromParent();
		DialogWidgetInstance = nullptr;
	}

	// #3: 퀘스트 선택 위젯 정리
	if (QuestSelectWidgetInstance)
	{
		QuestSelectWidgetInstance->RemoveFromParent();
		QuestSelectWidgetInstance = nullptr;
	}

	// #4: 게임 모드 설정 (입력 등)
	RestoreGameplayMode();

	// #5: 상태 초기화
	GetWorld()->GetTimerManager().SetTimer(ViewTargetBlendTimer,
		FTimerDelegate::CreateWeakLambda(this, [this]()
			{
				bInteracting = false;
				InteractorPawn = nullptr;
				PC = nullptr;
				OriginalViewTarget = nullptr;
			}),
		ViewTargetBlendTime,
		false
	);
}

void AQuestGiverActor::HandleQuestAccepted(FName QuestID)
{
	if (!InteractorPawn) return;

	// 플레이어의 퀘스트 컴포넌트에 퀘스트 데이터 전달(등록)
	if (FQuestData* Row = QuestDataTables->FindRow<FQuestData>(QuestID, QuestGiverActorContext))
	{
		// 플레이어의 퀘스트 컴포넌트 찾기
		if (UQuestComponent* QuestComponent = InteractorPawn->FindComponentByClass<UQuestComponent>())
		{
			// 퀘스트 데이터 전달(등록)
			QuestComponent->AcceptQuest(*Row);
		}
	}

	// 이 NPC에서 제공하는 퀘스트 리스트에서 수락한 퀘스트 제거
	//ProvidedQuestIDs.Remove(QuestID);
}

void AQuestGiverActor::HandleQuestTurnIn(FName QuestID)
{
	if (!InteractorPawn) return;

	if (UQuestComponent* QuestComp = InteractorPawn->FindComponentByClass<UQuestComponent>())
	{
		if (QuestComp->TurnInQuest(QuestID))
		{
			// 이 NPC에서 제공하는 퀘스트 리스트에서 완료한 퀘스트 제거
			ProvidedQuestIDs.Remove(QuestID);
		}
	}
}

void AQuestGiverActor::BeginPlay()
{
	Super::BeginPlay();

	// 위젯 컴포넌트 빌보드 설정
	if (UWorld* World = GetWorld())
	{
		const float UpdateInterval = 0.04f;

		World->GetTimerManager().SetTimer(
			WidgetComponentBillboardTimerHandle,
			this,
			&AQuestGiverActor::WidgetComponentBillboard,
			UpdateInterval,
			true
		);
	}

	InteractionComponent->SetInteractCollision(InteractionVolume, ObjectName);
}

TArray<FQuestData> AQuestGiverActor::GetProvidedQuests() const
{
	// 이 NPC가 제공할 QuestID 리스트
	TArray<FQuestData> ProvidedQuests;

	// 퀘스트 데이터 테이블이 없으면 반환
	if (!QuestDataTables)
	{
		return ProvidedQuests;
	}

	// ProvidedQuestIDs가 비어있는 경우
	if (ProvidedQuestIDs.Num() == 0)
	{
		static const FString ContextString(TEXT("QuestGiver_GetAll"));
		TArray<FName> RowNames = QuestDataTables->GetRowNames();

		// DataTable의 모든 행을 QuestID 리스트에 추가
		for (const FName& QuestID : RowNames)
		{
			FQuestData* Data = QuestDataTables->FindRow<FQuestData>(QuestID, ContextString);
			if (Data)
			{
				ProvidedQuests.Add(*Data);
			}
		}
	}
	else
	{
		static const FString ContextString(TEXT("QuestGiver_GetProvided"));

		// 제공할 QuestID 리스트만 QuestID 리스트에 추가
		for (const FName& QuestID : ProvidedQuestIDs)
		{
			FQuestData* Data = QuestDataTables->FindRow<FQuestData>(QuestID, ContextString);
			if (Data)
			{
				ProvidedQuests.Add(*Data);
			}
		}
	}

	return ProvidedQuests;
}

void AQuestGiverActor::WidgetComponentBillboard()
{
	if (!NPCWidgetComponent || !GetWorld()) return;

	if (!PC)
	{
		PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	}
	if (!PC) return;

	// 플레이어 카메라 위치
	FVector CameraLocation;
	FRotator CameraRotation;
	PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// 위젯 컴포넌트 위치
	const FVector WidgetLocation = NPCWidgetComponent->GetComponentLocation();

	// 위젯 → 카메라 방향을 향하는 회전 계산
	FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(WidgetLocation, CameraLocation);

	// Pitch/Roll 고정
	TargetRot.Pitch = 0.f;
	TargetRot.Roll = 0.f;

	// 현재 회전
	const FRotator CurrentRot = NPCWidgetComponent->GetComponentRotation();

	// DeltaTime은 World에서 가져옴
	const float DeltaTime = GetWorld()->GetDeltaSeconds();

	// 회전 보간
	const FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, 5.f);

	// 적용
	NPCWidgetComponent->SetWorldRotation(NewRot);
}

void AQuestGiverActor::ApplyUIInteractionMode()
{
	if (!InteractorPawn || !PC) return;

	// 상호작용 플레이어 메시 비활성화
	USkeletalMeshComponent* Mesh = InteractorPawn->FindComponentByClass<USkeletalMeshComponent>();
	if (Mesh)
	{
		Mesh->SetVisibility(false, true);
	}

	// 커서 활성화 및 컨트롤러 입력 비활성화
	PC->bShowMouseCursor = true;
	PC->SetIgnoreLookInput(true);
	PC->SetIgnoreMoveInput(true);
	PC->bEnableClickEvents = false;
	PC->bEnableMouseOverEvents = false;

	/* 플레이어 HUD 비가시화 */
	TArray<FWidgetVisibilityRecord> Records;
	TArray<UUserWidget*> AllWidgets;

	// 월드 전체에서 위젯 클래스를 찾아서 저장
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(PC->GetWorld(), AllWidgets, UUserWidget::StaticClass(), false);

	for (UUserWidget* Widget : AllWidgets)
	{
		if (!Widget) continue;

		// 소유 플레이어가 동일한지 확인
		if (Widget->GetOwningPlayer() && Widget->GetOwningPlayer()->IsA(PC->GetClass()))
		{
			// 위젯 가시화 기록 구조체에 가시화 정보 저장
			FWidgetVisibilityRecord Rec;
			Rec.Widget = Widget;
			Rec.PrevVisibility = Widget->GetVisibility();
			Widget->SetVisibility(ESlateVisibility::Collapsed);
			Records.Add(MoveTemp(Rec));
		}
	}

	// 가시화가 변경된 위젯이 있는 경우
	if (Records.Num() > 0)
	{
		// 변경 위젯 갱신 (이동으로 최적화)
		CachedWidgetStates.Add(PC, MoveTemp(Records));
	}

	// 입력 모드 UI Only로 설정 및 다이얼로그 위젯으로 포커싱
	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(DialogWidgetInstance ? DialogWidgetInstance->TakeWidget() : TSharedPtr<SWidget>());
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PC->SetInputMode(Mode);
}

void AQuestGiverActor::RestoreGameplayMode()
{
	if (!InteractorPawn || !PC) return;

	// 상호작용 플레이어 메시 활성화
	USkeletalMeshComponent* Mesh = InteractorPawn->FindComponentByClass<USkeletalMeshComponent>();
	if (Mesh)
	{
		Mesh->SetVisibility(true, true);
	}

	// 커서 비활성화 및 컨트롤러 입력 활성화
	PC->bShowMouseCursor = false;
	PC->SetIgnoreLookInput(false);
	PC->SetIgnoreMoveInput(false);
	PC->bEnableClickEvents = true;
	PC->bEnableMouseOverEvents = true;

	// 플레이어 HUD 비가시화
	TArray<FWidgetVisibilityRecord>* Found = CachedWidgetStates.Find(PC);
	if (!Found) return;

	for (const FWidgetVisibilityRecord& Rec : *Found)
	{
		if (Rec.Widget.IsValid())
		{
			UUserWidget* W = Rec.Widget.Get();
			// 위젯이 여전히 존재하면 이전 상태로 복원
			W->SetVisibility(Rec.PrevVisibility);
		}
		// 만약 위젯이 파괴되었으면 무시
	}

	CachedWidgetStates.Remove(PC);

	// 입력 모드 게임 모드로 복귀
	FInputModeGameOnly Mode;
	PC->SetInputMode(Mode);
}
