// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tank.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles//ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine//World.h"
#include "Projectile.generated.h"

UCLASS()
class MYPROJECT_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	ATank* ProjectileParent = nullptr;

	virtual  void Destroyed() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetProjectileParent(ATank* Parent);

	UPROPERTY(VisibleAnywhere,Category="Component")
	UProjectileMovementComponent *ProjectileMovementComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere,Category="Component")
	URadialForceComponent* ExplosionForce = nullptr;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Component")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Component")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere,Category="Effects")
	UParticleSystem* ExplosionEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float ProjectileDamage = 20.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<class UDamageType> DamageType;

	UFUNCTION(Category="Projectile")
	void OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	void LaunchProjectile(float Speed);

	
};
