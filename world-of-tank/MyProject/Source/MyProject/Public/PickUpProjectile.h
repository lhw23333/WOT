// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUp.h"
#include "PickUpProjectile.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API APickUpProjectile : public APickUp
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EProjectileType ProjectileType;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int Count;
};
