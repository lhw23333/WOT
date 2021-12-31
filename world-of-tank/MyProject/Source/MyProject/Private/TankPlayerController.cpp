// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"

#include "DrawDebugHelpers.h"
#include "TankAIController.h"
#include "GameFramework/PlayerStart.h"

ATank* ATankPlayerController::GetControlledTank()
{
	return Cast<ATank>(GetPawn());
}

ATankVehicle* ATankPlayerController::GetControlledTankVehicle()
{
	return Cast<ATankVehicle>(GetPawn());
}


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ATank* controllerTank = GetControlledTank();
}

void ATankPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if(GetLocalPlayer())
	{
		UE_LOG(LogTemp,Warning,TEXT("playerLocal is %s"),*GetLocalPlayer()->GetName());
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("playerLocal is null"));
	}
		
	
	if(InPawn)
	{
		auto PossessedTank = Cast<ATankVehicle>(InPawn);
		if(!PossessedTank)
			return;
		PossessedTank->OnDeath.AddUniqueDynamic(this,&ATankPlayerController::OnControlledTankDeath);
		bPawnIsLive = true;
	}
}

void ATankPlayerController::OnControlledTankDeath()
{
	//player death
	if(GetPawn()->IsLocallyControlled())
	{
		if(GetControlledTankVehicle())
			GetControlledTankVehicle()->DetachFromControllerPendingDestroy();
        
		UE_LOG(LogTemp,Warning ,TEXT("client tank dead"));
	}
	if(GetLocalRole() == ROLE_Authority)
	{
		//UE_LOG(LogTemp,Warning ,TEXT("Server tank dead"));
		bPawnIsLive = false;
		OnPawnDeath.Broadcast();
		
	}
	//GetControlledTank()->DetachFromControllerPendingDestroy();
	
	//tank respawn
	//TArray<AActor*> PlayerStarts;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(),APlayerStart::StaticClass(),PlayerStarts);
	//延迟执行
	
}



void ATankPlayerController::Tick(float DeltaSeconds)
{
	//move to aim
	AimToTarget();
}

void ATankPlayerController::AimToTarget()
{
	if(!GetPawn()) {return;}
	FVector HitLocation;
	if(GetSightRayHitLocation(HitLocation))
	{
		//UE_LOG(LogTemp,Warning,TEXT("HitLocation is %s"),*HitLocation.ToString());
		
		//if(GetControlledTank())
		{
			//GetControlledTank()->FindComponentByClass<UTankAimingComponent>()->AimAt(HitLocation);
		}
		if(GetControlledTankVehicle())
		{
			GetControlledTankVehicle()->FindComponentByClass<UTankAimingComponent>()->AimAt(HitLocation);
		}
		
	}

	
}

/*获取碰撞点*/
bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation)
{
	int32 ViewportSizeX,ViewportSizeY ;

	GetViewportSize(ViewportSizeX,ViewportSizeY);//output 参数

	FVector2D ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation,ViewportSizeY * CrosshairYLocation);

	FVector WorldLocation;
	FVector WorldDirection;
	
	if(DeprojectScreenPositionToWorld(ScreenLocation.X,ScreenLocation.Y,WorldLocation,WorldDirection))
	{
		GetLookVectorHitLocation(WorldDirection,OutHitLocation);
		return true;
	}
	return false;
}


bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& OutHitLocation)
{
	FVector StartLocation = PlayerCameraManager->GetCameraLocation();
	
	FVector EndLocation = StartLocation + LookDirection* LineTraceRange;

	FHitResult HitResult;
	
	if(GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Pawn))
	{
		OutHitLocation = HitResult.Location;
		//射线检测
		/*DrawDebugLine(
				GetWorld(),
				StartLocation,
				HitResult.Location,
				FColor::Red,
				false,
				0.0f,
				0.0f,
				10.0f);*/
		
		return true;
	}
	OutHitLocation = FVector(0.0f);
	
	return false;
}





