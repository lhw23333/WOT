// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "WeaponAttributeSet.h"

// Sets default values for this component's properties
//UInventoryComponent::UInventoryComponent()
//{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

//}

UInventoryComponent::UInventoryComponent(const FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentProjectileType = EProjectileType::AP;

	
	
	
	
	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

