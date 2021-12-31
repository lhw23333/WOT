// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "TankTurret.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom),meta=(BlueprintSpawnableComponent))
class MYPROJECT_API UTankTurret : public UStaticMeshComponent
{
	GENERATED_BODY()
protected:	
	UPROPERTY(EditAnywhere,Category="SetUp")
	float MaxDegreesPreSecond = 20.0f;

	virtual  void BeginPlay() override;
public:
	UFUNCTION(Server, Reliable)
	void MoveTurret(FVector AimDirection);
	
	float GetChangeYaw();
	
	float ChangeYaw;

	
};
