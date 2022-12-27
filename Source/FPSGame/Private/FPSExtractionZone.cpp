// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractionZone.h"

#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "UnrealNetwork.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	TriggerBox->SetupAttachment(RootComponent);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	DecalComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFPSExtractionZone::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::OnOverlapBegin);
}

void AFPSExtractionZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFPSCharacter* Character = nullptr;

	if (OtherActor && OtherActor != this && ObjectiveMissingSound)
	{
		Character = Cast<AFPSCharacter>(OtherActor);

		if (Character == nullptr)
		{
			return;
		}
	}

	if (Character->IsCarryingObjective())
	{
		AFPSGameMode* GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());

		if (GameMode)
		{
			GameMode->FinishMission(Character, true);
		}
	}

	else
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ObjectiveMissingSound);
	}
}