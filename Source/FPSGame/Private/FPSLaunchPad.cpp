// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSLaunchPad.h"

#include "FPSCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LaunchTrigger"));
	TriggerBox->SetupAttachment(RootComponent);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("LaunchAngle"));
	ArrowComponent->SetupAttachment(RootComponent);

	LaunchStrength = 1000;
}

// Called when the game starts or when spawned
void AFPSLaunchPad::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::OnOverlapBegin);
}

void AFPSLaunchPad::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFPSCharacter* Character;

	if (OtherActor && OtherActor != this)
	{
		Character = Cast<AFPSCharacter>(OtherActor);
	}

	else
	{
		return;
	}

	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch = ArrowComponent->GetComponentRotation().Pitch;

	const FVector LaunchVelocity = LaunchDirection.Vector() * LaunchStrength;

	if (Character)
	{
		Character->LaunchCharacter(LaunchVelocity, true, true);
	}

	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);
	}

	if (ActivateLaunchPadEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchPadEffect, GetActorLocation());
	}
}