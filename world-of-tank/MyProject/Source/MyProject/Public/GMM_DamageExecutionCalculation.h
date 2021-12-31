// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "GMM_DamageExecutionCalculation.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UGMM_DamageExecutionCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UGMM_DamageExecutionCalculation();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

	
};
