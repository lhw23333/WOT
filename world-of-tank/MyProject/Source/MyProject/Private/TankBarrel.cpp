// Fill out your copyright notice in the Description page of Project Settings.


#include "TankBarrel.h"

//老版本使用的函数
void UTankBarrel::MoveBarrel_Implementation(FVector AimDirection)
{
	FRotator CurrentRotation = GetForwardVector().Rotation();
	FRotator AimRotation = AimDirection.Rotation();

	float ChangePitch = AimRotation.Pitch - CurrentRotation.Pitch;

	float RelativeSpeed = FMath::Clamp<float>(ChangePitch,-1,1);
	
	float RotationChange = RelativeSpeed * MaxDegreesPreSecond *GetWorld()->DeltaTimeSeconds;

	float NewRotationPitch = RotationChange + GetRelativeRotation().Pitch;

	NewRotationPitch = FMath::Clamp<float>(NewRotationPitch,MinDegrees,MaxDegrees);
	
	SetRelativeRotation(FRotator(NewRotationPitch,0,0));
}

void UTankBarrel::BeginPlay()
{
	SetIsReplicated(true);
}



