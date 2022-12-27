// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGameState.h"

#include "EngineUtils.h"
#include "FPSCharacter.h"
#include "FPSPlayerController.h"

void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bMissionSuccess)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AFPSPlayerController* PlayerController = Cast<AFPSPlayerController>(It->Get());

		if (PlayerController && PlayerController->IsLocalController())
		{
			PlayerController->OnMissionCompleted(InstigatorPawn, bMissionSuccess);

			APawn* Pawn = PlayerController->GetPawn();

			if (Pawn)
			{
				Pawn->DisableInput(PlayerController);
			}
		}
	}
}