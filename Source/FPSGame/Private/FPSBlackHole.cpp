// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSBlackHole.h"

#include "Components/SphereComponent.h"


// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PhysicsZone"));
	TriggerSphere->SetupAttachment(RootComponent);
	TriggerSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	ForceStrength = 2000;
}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();

	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OnMeshOverlapBegin);
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingComponents;
	TriggerSphere->GetOverlappingComponents(OverlappingComponents);

	for (int i = 0; i < OverlappingComponents.Num(); ++i)
	{
		if (OverlappingComponents[i] == nullptr || !OverlappingComponents[i]->IsSimulatingPhysics())
		{
			continue;
		}

		OverlappingComponents[i]->AddRadialForce(GetActorLocation(), TriggerSphere->GetScaledSphereRadius(),
			-ForceStrength, RIF_Constant, true);
	}
}

void AFPSBlackHole::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		OtherActor->Destroy();
	}
}