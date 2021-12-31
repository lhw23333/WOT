// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTurret.h"

#include "Net/UnrealNetwork.h"

/*void UTankTurret::MoveTurret(FVector AimDirection)
{
	FRotator CurrentRotation = GetForwardVector().Rotation();
	FRotator AimRotation = AimDirection.Rotation();

	ChangeYaw = AimRotation.Yaw - CurrentRotation.Yaw;

	if(ChangeYaw >180)
	{
		ChangeYaw =  ChangeYaw - 360;
	}
	else if(ChangeYaw < -180)
	{
		ChangeYaw = ChangeYaw + 360;
	}
	
	float RelativeSpeed = FMath::Clamp<float>(ChangeYaw,-1,1);

	float RotationChange = RelativeSpeed * MaxDegreesPreSecond *GetWorld()->DeltaTimeSeconds;

	
	
	float NewRotationYaw = RotationChange + GetRelativeRotation().Yaw;

	SetRelativeRotation(FRotator(0,NewRotationYaw,0));

	//MoveServerTurrent(AimDirection);
}*/

void UTankTurret::MoveTurret_Implementation(FVector AimDirection)
{
	FRotator CurrentRotation = GetForwardVector().Rotation();
	FRotator AimRotation = AimDirection.Rotation();

	ChangeYaw = AimRotation.Yaw - CurrentRotation.Yaw;

	if(ChangeYaw >180)
	{
		ChangeYaw =  ChangeYaw - 360;
	}
	else if(ChangeYaw < -180)
	{
		ChangeYaw = ChangeYaw + 360;
	}
	
	float RelativeSpeed = FMath::Clamp<float>(ChangeYaw,-1,1);

	float RotationChange = RelativeSpeed * MaxDegreesPreSecond *GetWorld()->DeltaTimeSeconds;

	
	
	float NewRotationYaw = RotationChange + GetRelativeRotation().Yaw;

	SetRelativeRotation(FRotator(0,NewRotationYaw,0));
}

void UTankTurret::BeginPlay()
{
	SetIsReplicated(true);
}



float UTankTurret::GetChangeYaw()
{
	
	return this->ChangeYaw;
}

