// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	// look for physics handle
	physHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!physHandle)
		UE_LOG(LogTemp, Error, TEXT("Physics handle not found for %s"), *(GetOwner()->GetName()));

	// look for input component handle
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent)
	{
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input component not found for %s"), *(GetOwner()->GetName()));
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if phys handle is attached, move the thing we're holding
	if (physHandle && physHandle->GrabbedComponent)
	{
		FVector outLocation;
		FRotator outRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(outLocation, outRotation);
		FVector lineTraceEnd = outLocation + outRotation.Vector() * reach;

		physHandle->SetTargetLocation(lineTraceEnd);
	}
}

void UGrabber::Grab()
{
	FHitResult hitRes = GetGrabbableObject();
	UPrimitiveComponent* compToGrab = hitRes.GetComponent();
	AActor* hitActor = hitRes.GetActor();

	// if we hit something, attach phys handle
	if (hitActor && physHandle)
	{
		physHandle->GrabComponentAtLocationWithRotation
		(
			compToGrab,
			NAME_None,
			compToGrab->GetOwner()->GetActorLocation(),
			compToGrab->GetOwner()->GetActorRotation()
		);
	}
}

void UGrabber::Release()
{
	if (physHandle)
		physHandle->ReleaseComponent();
}

FHitResult UGrabber::GetGrabbableObject()
{
	// get player view point
	FVector outLocation;
	FRotator outRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(outLocation, outRotation);

	// ray cast to set reach distance
	FVector lineTraceEnd = outLocation + outRotation.Vector() * reach;

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

	return outHit;
}
