// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"

#include "FPSGameMode.h"
#include "UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/PawnSensingComponent.h"



// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensor"));

	GuardState = EAIState::Idle;

	bReplicates = true;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComponent->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiceHeared);

	OriginalRotation = GetActorRotation();
}


void AFPSAIGuard::OnPawnSeen(APawn* SeePawn)
{
	if (SeePawn == nullptr)
	{
		return;
	}

	AFPSGameMode* GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->FinishMission(SeePawn, false);

	SetGuardState(EAIState::Alerted);
}

void AFPSAIGuard::OnNoiceHeared(APawn* NoiceInstigator, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	const FVector Direction = Location - GetActorLocation();
	FRotator NewRotation = UKismetMathLibrary::MakeRotFromX(Direction);

	NewRotation.Pitch = 0;
	NewRotation.Roll = 0;

	SetActorRotation(NewRotation);

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetRotation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetRotation, this, &AFPSAIGuard::ResetRotation, 3);

	SetGuardState(EAIState::Suspicious);
}

void AFPSAIGuard::ResetRotation()
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	SetActorRotation(OriginalRotation);
	SetGuardState(EAIState::Idle);
}



void AFPSAIGuard::SetGuardState(EAIState NewGuardState)
{
	if (GuardState == NewGuardState)
	{
		return;
	}

	GuardState = NewGuardState;
	OnStateChanged(GuardState);
}



void AFPSAIGuard::OnRep_GuardState()
{
	OnStateChanged(GuardState);
}

void AFPSAIGuard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSAIGuard, GuardState);
}