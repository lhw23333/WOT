// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "TankAimingComponent.generated.h"


UENUM()
enum class EFiringState :uint8
{
	Reloading,
	Aiming
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "c++fun")
	void Initialize(UTankBarrel* BarrelToSet,UTankTurret* TurretToSet,USkeletalMeshComponent* SkeletalMesh,UParticleSystemComponent* FireParticleToSet,UParticleSystemComponent* SmokeParticleToSet);
	
	void AimAt(FVector HitLocation);

	UTankBarrel* Barrel = nullptr;
	
	UTankTurret* Turret = nullptr;

	USkeletalMeshComponent *TankMesh = nullptr;
	
	UPROPERTY(EditAnywhere,Category="SetUp")
	float LaunchSpeed = 10000.0f;
	


	float GetChangeYaw();

#pragma region  FireVector同步
	UPROPERTY(BlueprintReadWrite)
	float ChangeYaw;
	/**同步firevector 控制炮塔*/
	UPROPERTY(ReplicatedUsing=OnRep_FireVector,EditAnywhere)
	FVector FireVector;

	void OnFireVectorUpdate();
	
	UFUNCTION()
	void OnRep_FireVector();
	
	UFUNCTION(BlueprintPure,Category="FireState")
	FVector GetFireVector();

	UFUNCTION(BlueprintCallable,Category="FireState",Server,Reliable)
	void SetFireVector(FVector NewFireVector);
#pragma endregion
	
    
	
    
#pragma region 开火特效
	UPROPERTY(EditDefaultsOnly)
	UParticleSystemComponent* FireImpactParticle;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystemComponent* SmokeImpactParticle;
#pragma endregion 

	//修改weapon Attribute 并active技能
	void ChangeWeaponAttribute();
	
	UPROPERTY(BlueprintReadOnly)
	EFiringState FiringState = EFiringState::Aiming;

	UPROPERTY(Category="Gameplay|Projectile",EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<class AProjectile> ProjectileClass;

	/** 射击之间的延迟，单位为秒。用于控制测试发射物的射击速度，还可防止服务器函数的溢出导致将SpawnProjectile直接绑定至输入。*/
	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	float FireRate;

	virtual  void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	/** 若为true，则正在发射投射物。*/
	UPROPERTY(ReplicatedUsing=OnRep_bIsFiringWeapon,EditAnywhere)
	bool bIsFiringWeapon;

	UFUNCTION(BlueprintPure,Category="FireState")
	FORCEINLINE bool GetIsFiringWeapon() const{return  bIsFiringWeapon;}

	UFUNCTION(BlueprintCallable,Category="FireState",Server,Reliable)
	void SetIsFiringWeapon(bool FiringWeapon);
	
	void OnIsFiringWeaponUpdate();

	UFUNCTION()
	void OnRep_bIsFiringWeapon();

	/** 用于启动武器射击的函数。*/
	UFUNCTION(BlueprintCallable, Category="Gameplay")
	void StartFire();

	/** 用于结束武器射击的函数。一旦调用这段代码，玩家可再次使用StartFire。*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopFire();  

	/** 用于生成投射物的服务器函数。*/
	UFUNCTION(BlueprintCallable)
	void HandleFire();
	
	/** 定时器句柄，用于提供生成间隔时间内的射速延迟。*/
	FTimerHandle FiringTimer;
private:
	UPROPERTY(EditAnywhere,Category="SetUp")
	double LastFireTime = 3.0f;
};
