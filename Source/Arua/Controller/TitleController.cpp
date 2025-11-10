// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"

void ATitleController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, ACameraActor::StaticClass(), FName("TitleCamera"), FoundCameras);

	if (!FoundCameras.IsEmpty())
	{
		SetViewTarget(FoundCameras[0]);
	}
}
