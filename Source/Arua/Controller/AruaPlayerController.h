// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AruaPlayerController.generated.h"

class UPlayerViewModel;
class UPlayerHUDView;
/**
 * Player에게 빙의할 Controller 클래스 입니다.
 * 작성자 : 임희섭
 * 작성일 : 25/11/05
 */
UCLASS()
class ARUA_API AAruaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAruaPlayerController();

public:
	void SetTargetBoss(class AARMonsterBase* InTargetMonster);
	void TargetOff();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UPlayerHUDView> HUDClass;

	// 퀘스트 HUD 위젯 클래스 (플레이어 화면에 현재 진행중인 퀘스트 목록과 진행 상황을 띄우는 위젯)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UQuestHUDWidget> QuestHUDClass;

	// 퀘스트 저널 위젯 클래스 (플레이어가 퀘스트 저널 토글 시, 현재 진행중인 퀘스트들의 목록과 상세 정보를 뛰우는 위젯 클래스)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UQuestJournalWidget> QuestJournalClass;

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;
	

private:
	UPROPERTY()
	TObjectPtr<UPlayerViewModel> ViewModel;

	UPROPERTY()
	TObjectPtr<UPlayerHUDView> HUD;

	// 퀘스트 HUD 위젯
	UPROPERTY()
	class UQuestHUDWidget* QuestHUD = nullptr;

	UPROPERTY()
	class UQuestJournalWidget* QuestJournal = nullptr;
};
