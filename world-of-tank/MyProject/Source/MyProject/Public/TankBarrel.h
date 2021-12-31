// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom),meta=(BlueprintSpawnableComponent))
class MYPROJECT_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,Category="SetUp")
	float MaxDegreesPreSecond = 20.0f;

	UPROPERTY(EditAnywhere,Category="SetUp")
	float MaxDegrees = 40.0f;
	
	UPROPERTY(EditAnywhere,Category="SetUp")
	float MinDegrees = -5.0f;

public:
	UFUNCTION(Server, Reliable)
	void MoveBarrel(FVector Aimdirection);
protected:
	virtual void BeginPlay() override;
};
