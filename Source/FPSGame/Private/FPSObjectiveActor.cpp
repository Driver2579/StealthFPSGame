// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSObjectiveActor.h"

#include "FPSCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	SphereComponent->SetupAttachment(MeshComponent);

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();

	PlayEffects();
}

void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AFPSCharacter* Character = Cast<AFPSCharacter>(OtherActor);

	if (Character == nullptr)
	{
		return;
	}

	PlayEffects();

	if (GetLocalRole() == ROLE_Authority)
	{
		Character->SetIsCarryingObjective(true);
		Destroy();
	}
}

void AFPSObjectiveActor::PlayEffects() const
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickupFX, GetActorLocation());
}