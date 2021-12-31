// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	MaxHp = 100;
	CurrentHp = MaxHp;
	
	
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ATank::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//护甲计算
	float damageApplied = CurrentHp - DamageAmount;
	SetCurrentHealth(damageApplied);
	return damageApplied;
}

float ATank::GetHeathPercentage()
{
	return CurrentHp/MaxHp;
}

void ATank::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	/**需要复制的属性*/
	DOREPLIFETIME(ATank,CurrentHp);
}

void ATank::OnHealthUpdate()
{
	//客户端特定功能
	if(IsLocallyControlled())
	{
		if(CurrentHp <= 0)
		{
			UE_LOG(LogTemp,Warning ,TEXT("tank dead"));
			OnDeath.Broadcast();
		}
	}

	//服务端特定功能
	if(GetLocalRole() == ROLE_Authority)
	{
		if(CurrentHp <= 0)
		{
			OnDeath.Broadcast();
		}
	}
	UE_LOG(LogTemp,Warning,TEXT("health"));
	//在所有机器上都执行的函数。 
	/*  
		因任何因伤害或死亡而产生的特殊功能都应放在这里。

		暂定 死亡爆炸特效
	*/
}


void ATank::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

void ATank::SetCurrentHealth(float healthValue)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		
		CurrentHp = FMath::Clamp(healthValue,0.f,MaxHp);
		OnHealthUpdate();
		
	}
	
}






