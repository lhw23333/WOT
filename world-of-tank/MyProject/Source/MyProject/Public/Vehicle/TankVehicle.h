// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "WheeledVehicle.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "AttributeSetBase.h"
#include "ProjectileAttributeSet.h"
#include "WeaponAttributeSet.h"
#include "InventoryComponent.h"
#include "TankVehicle.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankVehicleDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangeEvent, float, NewHealth);

UCLASS()
class MYPROJECT_API ATankVehicle : public AWheeledVehicle,public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ATankVehicle();
	
#pragma region GAS系统
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Abilities")
	UAbilitySystemComponent* AbilitySystem;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category="Ability System")
	void GiveAbility(TSubclassOf<UGameplayAbility> Ability);

	//基础血量和护甲值
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Abilities")
	UAttributeSetBase* AttributeSet;

	//weapon攻击力
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Abilities")
	FGameplayTag Tag;

	UPROPERTY(BlueprintAssignable, Category="Ability")
	FOnAttributeChangeEvent HealthChangeEvent;

	UPROPERTY(BlueprintAssignable, Category="Ability")
	FOnAttributeChangeEvent AromrChangeEvent;

	
	
	//触发change事件
	void OnHealthAttributeChanged(const FOnAttributeChangeData& Data);
	void OnAromrAttributeChanged(const FOnAttributeChangeData& Data);
	
	

	
#pragma endregion 
	
#pragma region  简易填装系统
	UPROPERTY(EditAnywhere,Category="Ability",ReplicatedUsing = OnRep_CurrentProject)
	EProjectileType CurrentProject = EProjectileType::AP;

	UFUNCTION()
	void OnRep_CurrentProject();
	
	void OnCurrent_Update();
	
	UFUNCTION(BlueprintCallable)
	EProjectileType GetCurrentProject();

	UFUNCTION(BlueprintCallable,Server,Reliable)
	void SetCurrentProject(EProjectileType NewType);
	
	UPROPERTY(BlueprintAssignable, Category="Ability")
	FOnAttributeChangeEvent AP_CountChangeEvent;

	UPROPERTY(BlueprintAssignable, Category="Ability")
	FOnAttributeChangeEvent APCR_CountChangeEvent;

	UPROPERTY(BlueprintAssignable, Category="Ability")
	FOnAttributeChangeEvent HE_CountChangeEvent;

	//修改子弹数量
	UFUNCTION(BlueprintCallable,Server,Reliable)
	void SetAP_Count(float AddValue);

	UFUNCTION(BlueprintCallable,Server,Reliable)
	void SetAPCR_Count(float AddValue);

	UFUNCTION(BlueprintCallable,Server,Reliable)
	void SetHE_Count(float AddValue);
	//触发子弹更新事件
	void OnAP_CountAttributeChanged(const FOnAttributeChangeData& Data);
	void OnAPCR_CountAttributeChanged(const FOnAttributeChangeData& Data);
	void OnHE_CountAttributeChanged(const FOnAttributeChangeData& Data);

	//pick up的overlap
	UFUNCTION()
	void OnOverLap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
#pragma endregion 
	/**自定义death事件*/
	UPROPERTY(BlueprintAssignable)
	FTankVehicleDelegate OnDeath;
	
	/**获取当前hp百分比*/
	UFUNCTION(BlueprintPure,Category="Tank")
	float GetHeathPercentage();
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


	
	
protected:
	virtual void BeginPlay() override;
	
	virtual  void Tick(float DeltaSeconds) override;

	/** 承受伤害的事件。从APawn覆盖。*/
	virtual  float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHp;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth,EditAnywhere)
	float CurrentHp = MaxHp;
	void OnHealthUpdate();
	UFUNCTION()
	void OnRep_CurrentHealth();
};
