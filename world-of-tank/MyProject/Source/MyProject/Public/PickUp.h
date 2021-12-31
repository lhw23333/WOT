// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "InventoryComponent.h"
#include "PickUp.generated.h"

UCLASS()
class MYPROJECT_API APickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUp();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Abilities")
	FGameplayTag Tag;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Abilities")
	UAbilitySystemComponent* AbilitySystem;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category="Ability System")
	void GiveAbility(TSubclassOf<UGameplayAbility> Ability);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};
