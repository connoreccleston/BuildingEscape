// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "DoorOpener.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UDoorOpener : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoorOpener();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FDoorEvent onOpen;

	UPROPERTY(BlueprintAssignable)
	FDoorEvent onClose;

private:
	//UPROPERTY(EditAnywhere)
	//float openAngle = 90.0f;
	
	UPROPERTY(EditAnywhere)
	ATriggerVolume* pressurePlate = nullptr;

	//UPROPERTY(EditAnywhere)
	//float closeDelay = 1.0f;

	UPROPERTY(EditAnywhere)
	float massThreshold = 100.0f;

	//float lastOpenTime;

	//FRotator closeRot;
	//FRotator openRot;

	float TotalMass();
};
