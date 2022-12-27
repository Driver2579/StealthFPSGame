// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"

#include "FPSGameState.h"
#include "FPSHUD.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();

	BlendTime = 0.5;
	BlendFunction = VTBlend_Cubic;
}

void AFPSGameMode::FinishMission(APawn* InstigatorPawn, bool bMissionSuccess)
{
	AFPSGameState* FPSGameState = GetGameState<AFPSGameState>();

	if (FPSGameState)
	{
		FPSGameState->MulticastOnMissionComplete(InstigatorPawn, bMissionSuccess);
	}

	if (InstigatorPawn == nullptr)
	{
		return;
	}

	OnMissionFinished(InstigatorPawn, bMissionSuccess);

	if (!SpectatingViewpointClass)
	{
		return;
	}

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, Actors);

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();

		if (PlayerController && !Actors.IsEmpty())
		{
			PlayerController->SetViewTargetWithBlend(Actors[0], BlendTime, BlendFunction);
		}
	}
}