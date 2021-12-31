// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "TankPlayerController.h"
#include "Vehicle/TankVehicle.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	ATankVehicle* ControllerTank = GetControlledTank();
	if(ControllerTank == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("is null"));
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Ai is %s"),*ControllerTank->GetName());
	}
}

void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if(InPawn)
	{
		auto PossessedTank = Cast<ATankVehicle>(InPawn);
		
		if(!PossessedTank)
			return;
		PossessedTank->OnDeath.AddUniqueDynamic(this,&ATankAIController::OnControlledTankDeath);
		bPawnIsLive = true;
	}
}


void ATankAIController::OnControlledTankDeath()
{
	UE_LOG(LogTemp,Warning,TEXT("%s is dead"),*GetControlledTank()->GetName());
	
	if(GetControlledTank())
	{
		GetControlledTank()->DetachFromControllerPendingDestroy();
		bPawnIsLive = false;
	}
}


void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//AimTarget();
	//MoveTo();
	
}

void ATankAIController::MoveTo()
{
	FVector TargetLocation = GetPlayerTank()->GetActorLocation();

	FVector CurrentLocation = GetControlledTank()->GetActorLocation();

	
	
}

/*瞄准玩家，并开火*/
void ATankAIController::AimTarget()
{
	if(!GetControlledTank() || !GetPlayerTank())
	{
		UE_LOG(LogTemp,Warning,TEXT("AItank is null"));
		return;
	}
	UE_LOG(LogTemp,Warning,TEXT("AItank is ok"));
	
	
	//瞄准
	auto AimingComponent = GetControlledTank()->FindComponentByClass<UTankAimingComponent>();

	if(AimingComponent->GetChangeYaw() < 10.0f)
	{
		UE_LOG(LogTemp,Warning,TEXT("Aim is %f"),AimingComponent->GetChangeYaw());
		if(Cast<ATankPlayerController>(GetPlayerTank()->GetController())->bPawnIsLive)
		{
			UE_LOG(LogTemp,Warning,TEXT("player tank is dead by ai"));
			AimingComponent->StartFire();
		}
		
		
	}

	
	AimingComponent->AimAt(GetPlayerTank()->GetActorLocation());
	return;
}




ATankVehicle* ATankAIController::GetControlledTank()
{
	return Cast<ATankVehicle>(GetPawn());
}

ATankVehicle* ATankAIController::GetPlayerTank()
{
	ATankVehicle* PlayerTank;
	if(GetWorld()->GetFirstPlayerController())
	{
		PlayerTank =Cast<ATankVehicle>(GetWorld()->GetFirstPlayerController()->GetPawn()); 
		return PlayerTank;
	}
	return nullptr;
	
	 
}
