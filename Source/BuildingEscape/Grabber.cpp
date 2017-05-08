// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber Begin Play"));
	
	// look for physics handle
	physHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!physHandle)
		UE_LOG(LogTemp, Error, TEXT("Physics handle not found for %s"), *(GetOwner()->GetName()));

	// look for input component handle
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent)
	{
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input component not found for %s"), *(GetOwner()->GetName()));
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("grabby grabby"));
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// get play view point
	FVector outLocation;
	FRotator outRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(outLocation, outRotation);
	//UE_LOG(LogTemp, Warning, TEXT("%s %s"), *outLocation.ToString(), *outRotation.ToString());

	// ray cast to set reach distance
	FVector lineTraceEnd = outLocation + outRotation.Vector() * reach;

	DrawDebugLine
	(
		GetWorld(),
		outLocation,
		lineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.0f,
		0.0f,
		10.0f
	);

	// setup query params
	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());

	FHitResult outHit;
	GetWorld()->LineTraceSingleByObjectType
	(
		outHit,
		outLocation,
		lineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParams
	);

	AActor* hitActor = outHit.GetActor();
	if (hitActor)
		UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *(hitActor->GetName()));

	// see what we hit
}

