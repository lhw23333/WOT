// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "Vehicle/TankVehicle.h"
#include "TankAIController.generated.h"


/**
 * 
 */
UCLASS()
class MYPROJECT_API ATankAIController : public AAIController
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "c++fun")
	ATankVehicle* GetControlledTank();

	UFUNCTION(BlueprintCallable, Category = "c++fun")
	ATankVehicle* GetPlayerTank();

	virtual void BeginPlay() override;

	virtual  void SetPawn(APawn* InPawn) override;
	
	virtual  void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void MoveTo();

	UFUNCTION(BlueprintCallable)
	void AimTarget();

	UPROPERTY(EditAnywhere,Category="Setup")
	float AcceptanceRadius = 3000.0f;

	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bPawnIsLive;
	UFUNCTION()
	void OnControlledTankDeath();
};
