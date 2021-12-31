// Fill out your copyright notice in the Description page of Project Settings.
#include "Vehicle/TankVehicle.h"
#include "GameplayTagContainer.h"
#include "PickUp.h"
#include "PickUpProjectile.h"
#include "Net/UnrealNetwork.h"

#include "Engine/Engine.h"
ATankVehicle::ATankVehicle()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	MaxHp = 100;
	CurrentHp = MaxHp;
	
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystem");

	AttributeSet = CreateDefaultSubobject<UAttributeSetBase>("Attribute Set");

	

	//Item_AP = CreateDefaultSubobject<UProjectileAttributeSet>("Item_AP");
	//Item_APCR = CreateDefaultSubobject<UProjectileAttributeSet>("Item_APCR");
	//Item_HE = CreateDefaultSubobject<UProjectileAttributeSet>("Item_HE");
	
	
	if(AbilitySystem)
	{
		AbilitySystem->GetGameplayAttributeValueChangeDelegate(UAttributeSetBase::GetHealthAttribute())
		.AddUObject(this, &ATankVehicle::OnHealthAttributeChanged);

		AbilitySystem->GetGameplayAttributeValueChangeDelegate(UAttributeSetBase::GetArmorAttribute())
		.AddUObject(this,&ATankVehicle::OnAromrAttributeChanged);

		AbilitySystem->GetGameplayAttributeValueChangeDelegate(UAttributeSetBase::GetAP_CountAttribute())
		.AddUObject(this,&ATankVehicle::OnAP_CountAttributeChanged);

		AbilitySystem->GetGameplayAttributeValueChangeDelegate(UAttributeSetBase::GetAPCR_CountAttribute())
		.AddUObject(this,&ATankVehicle::OnAPCR_CountAttributeChanged);

		AbilitySystem->GetGameplayAttributeValueChangeDelegate(UAttributeSetBase::GetHE_CountAttribute())
		.AddUObject(this,&ATankVehicle::OnHE_CountAttributeChanged);
		
	}

	
}

void ATankVehicle::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->OnComponentBeginOverlap.AddDynamic(this,&ATankVehicle::OnOverLap);
}

void ATankVehicle::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


float ATankVehicle::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//护甲计算
	if(GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp,Warning ,TEXT("other take damage"));
		float damageApplied = CurrentHp - DamageAmount;
        SetCurrentHealth(damageApplied);
        return damageApplied;
	}
	return -1.0f;
	
}

float ATankVehicle::GetHeathPercentage()
{
	return CurrentHp/MaxHp;
}

void ATankVehicle::SetCurrentHealth(float healthValue)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		
		CurrentHp = FMath::Clamp(healthValue,0.f,MaxHp);
		OnHealthUpdate();
		
	}
}


void ATankVehicle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	/**需要复制的属性*/
	DOREPLIFETIME(ATankVehicle,CurrentHp);
	DOREPLIFETIME(ATankVehicle,CurrentProject);
	
	
}

void ATankVehicle::OnCurrent_Update()
{
	
}

void ATankVehicle::OnRep_CurrentProject()
{
	OnCurrent_Update();
}

EProjectileType ATankVehicle::GetCurrentProject()
{
	return this->CurrentProject;
}

void ATankVehicle::SetCurrentProject_Implementation(EProjectileType NewType)
{
	CurrentProject = NewType;
	UE_LOG(LogTemp,Warning,TEXT("set ok"));
}



void ATankVehicle::OnHealthUpdate()
{
	//客户端特定功能
	if(IsLocallyControlled())
	{
		if(CurrentHp <= 0)
		{
			UE_LOG(LogTemp,Warning ,TEXT("tank dead"));
			OnDeath.Broadcast();
		}
		UE_LOG(LogTemp,Warning,TEXT("health %f"),CurrentHp);
	}

	//服务端特定功能
	if(GetLocalRole() == ROLE_Authority)
	{
		if(CurrentHp <= 0)
		{
			OnDeath.Broadcast();
		}
	}
	
	//在所有机器上都执行的函数。 
	/*  
		因任何因伤害或死亡而产生的特殊功能都应放在这里。

		暂定 死亡爆炸特效
	*/
}

void ATankVehicle::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

UAbilitySystemComponent* ATankVehicle::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

void ATankVehicle::GiveAbility(TSubclassOf<UGameplayAbility> Ability)
{
	if(AbilitySystem)
	{
		if(HasAuthority() && Ability)
		{
			AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability, 1));
			
		}
		AbilitySystem->InitAbilityActorInfo(this, this);
	}
}

void ATankVehicle::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
	HealthChangeEvent.Broadcast(Data.NewValue);
	UE_LOG(LogTemp,Warning,TEXT("health is %f"),Data.NewValue);
	if(Data.NewValue <= 0.0f)
	{
		OnDeath.Broadcast();
	}
	
	
}

void ATankVehicle::OnAromrAttributeChanged(const FOnAttributeChangeData& Data)
{
	AromrChangeEvent.Broadcast(Data.NewValue);
}

void ATankVehicle::OnAP_CountAttributeChanged(const FOnAttributeChangeData& Data)
{
	AP_CountChangeEvent.Broadcast(Data.NewValue);
}

void ATankVehicle::OnAPCR_CountAttributeChanged(const FOnAttributeChangeData& Data)
{
	APCR_CountChangeEvent.Broadcast(Data.NewValue);
}

void ATankVehicle::OnHE_CountAttributeChanged(const FOnAttributeChangeData& Data)
{
	HE_CountChangeEvent.Broadcast(Data.NewValue);
}
void ATankVehicle::SetAP_Count_Implementation(float AddValue)
{
	float temp = AttributeSet->GetAP_Count();
	AttributeSet->SetAP_Count(temp +  AddValue);
}

void ATankVehicle::SetAPCR_Count_Implementation(float AddValue)
{
	float temp = AttributeSet->GetAPCR_Count();
	AttributeSet->SetAPCR_Count(temp + AddValue);
}

void ATankVehicle::SetHE_Count_Implementation(float AddValue)
{
	float temp = AttributeSet->GetHE_Count();
	AttributeSet->SetHE_Count(temp + AddValue);
}



void ATankVehicle::OnOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		if(APickUp* PickUp = Cast<APickUp>(OtherActor))
		{
			if(PickUp->Tag == FGameplayTag::RequestGameplayTag(FName("Level.PickUp.Projectile")))
			{
				UE_LOG(LogTemp,Warning,TEXT("pickup projectile"));
				EProjectileType Type = Cast<APickUpProjectile>(PickUp)->ProjectileType;
				switch (Type)
				{
				case EProjectileType::AP:
					SetAP_Count(Cast<APickUpProjectile>(PickUp)->Count);
					break;
				case EProjectileType::APCR:
					SetAPCR_Count(Cast<APickUpProjectile>(PickUp)->Count);
					break;
				case EProjectileType::HE:
					SetHE_Count(Cast<APickUpProjectile>(PickUp)->Count);
					break;
					default:;
				}
			}
			if(PickUp->Tag == FGameplayTag::RequestGameplayTag(FName("Level.PickUp.ArmorBag")))
			{
				FGameplayTag tag = FGameplayTag::RequestGameplayTag(FName("Ability.PickUp.Buff"));
				FGameplayTagContainer tags(tag);
				AbilitySystem->TryActivateAbilitiesByTag(tags);
			}

			OtherActor->Destroy();
		}

		
	}
}



