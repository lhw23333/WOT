// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tank.h"
#include "TankAimingComponent.h"
#include "Engine/World.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Vehicle/TankVehicle.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FControlledDelegate);

UCLASS()
class MYPROJECT_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;

	virtual  void SetPawn(APawn* InPawn) override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable, Category = "c++fun")
	ATank* GetControlledTank();

	UFUNCTION(BlueprintCallable,Category= "c++fun")
	ATankVehicle* GetControlledTankVehicle();
	
	void AimToTarget();

	bool GetSightRayHitLocation(FVector &OutHitLocation);

	bool GetLookVectorHitLocation(FVector LookDirection,FVector &OutHitLocation);

	UFUNCTION()
	void OnControlledTankDeath();

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bPawnIsLive;

	UPROPERTY(BlueprintAssignable)
	FControlledDelegate OnPawnDeath;

private:
	float CrosshairXLocation=  0.5;
	float CrosshairYLocation = 0.35;
	UPROPERTY(EditAnywhere)
	float LineTraceRange = 1000000.0f;

	
};
