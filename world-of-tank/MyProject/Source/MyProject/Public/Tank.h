// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Actor.h>

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);

UCLASS()
class MYPROJECT_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	/** 承受伤害的事件。从APawn覆盖。*/
	virtual  float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	/**获取当前hp百分比*/
	UFUNCTION(BlueprintPure,Category="Tank")
	float GetHeathPercentage();
	/**自定义death事件*/
	UPROPERTY(BlueprintAssignable)
	FTankDelegate OnDeath;
	/**复制属性*/
	virtual  void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	/** 最大生命值的取值函数。*/
	UFUNCTION()
	FORCEINLINE float GetMaxHealth() const { return MaxHp; } 
	/** 当前生命值的取值函数。*/
	UFUNCTION(BlueprintPure, Category="Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHp; }
	/** 当前生命值的存值函数。将此值的范围限定在0到MaxHealth之间，并调用OnHealthUpdate。仅在服务器上调用。*/
	UFUNCTION(BlueprintCallable, Category="Health")
	void SetCurrentHealth(float healthValue);

	/**功率待开发*/
	float Power;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHp;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHp = MaxHp;
	//UPROPERTY(BlueprintReadWrite, Category = "Health")
	//int Group;
	
	/** 响应要更新的生命值。修改后，立即在服务器上调用，并在客户端上调用以响应RepNotify*/
	void OnHealthUpdate();
	
	UFUNCTION()
	void OnRep_CurrentHealth();

	
};
