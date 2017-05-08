// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "DoorOpener.h"


// Sets default values for this component's properties
UDoorOpener::UDoorOpener()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDoorOpener::BeginPlay()
{
	Super::BeginPlay();

	//whoOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

	closeRot = GetOwner()->GetActorRotation();
	openRot = GetOwner()->GetActorRotation().Add(0.0f, openAngle, 0.0f);
}

// Called every frame
void UDoorOpener::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMass() >= massThreshold)
	{
		GetOwner()->SetActorRotation(openRot);
		lastOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() >= lastOpenTime + closeDelay)
	{
		GetOwner()->SetActorRotation(closeRot);
	}

	//if(!pressurePlate->IsOverlappingActor(whoOpens))
	//	GetOwner()->SetActorRotation(closeRot);
}

float UDoorOpener::TotalMass()
{
	float totalMass = 0.0f;

	// find all overlapping actors, add their masses

	TArray<AActor*> overlappingActors;
	pressurePlate->GetOverlappingActors(overlappingActors);

	for (const auto* actor : overlappingActors)
		totalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();

	return totalMass;
}