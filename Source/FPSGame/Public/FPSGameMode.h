// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

UCLASS()
class AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFPSGameMode();

private:
	UPROPERTY(EditAnywhere, Category = "Blend")
	TSubclassOf<AActor> SpectatingViewpointClass;

	UPROPERTY(EditAnywhere, Category = "Blend")
	float BlendTime;

	UPROPERTY(EditAnywhere, Category = "Blend")
	TEnumAsByte<EViewTargetBlendFunction> BlendFunction;

protected:
	UFUNCTION(BlueprintImplementableEvent)
    void OnMissionFinished(APawn* InstigatorPawn, bool bMissionSuccess);

public:
	void FinishMission(APawn* InstigatorPawn, bool bMissionSuccess);
};