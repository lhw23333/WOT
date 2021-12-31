// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"
#include "DrawDebugHelpers.h"
#include "AttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "InventoryComponent.h"
#include "Projectile.h"
#include "TankAIController.h"
#include "Vehicle/TankVehicle.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//SetIsReplicated(true);
	// ...

	//初始化投射物类
	static  ConstructorHelpers::FClassFinder<AProjectile> BP_Projectile(TEXT("Blueprint'/Game/TankBase/BluePrints/Tank/Projectile/BP_Projectile.BP_Projectile_c'"));
	if(BP_Projectile.Succeeded())
	{
		ProjectileClass = BP_Projectile.Class;
	}
	//初始化射速
	FireRate = 1.0f;
	bIsFiringWeapon = false;
	
}


// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(Barrel == nullptr || Turret == nullptr)
		return;
	if(FPlatformTime::Seconds() - LastFireTime < FireRate)
	{
		FiringState = EFiringState::Reloading;
		return;
	}
	else
	{
		FiringState = EFiringState::Aiming;
	}
		
	
}

void UTankAimingComponent::Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet, USkeletalMeshComponent* SkeletalMesh, UParticleSystemComponent* FireParticleToSet, UParticleSystemComponent* SmokeParticleToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
	TankMesh = SkeletalMesh;

	FireImpactParticle = FireParticleToSet;
	SmokeImpactParticle = SmokeParticleToSet;

	SetIsReplicated(true);
}



void UTankAimingComponent::AimAt(FVector HitLocation)
{
	//if(Barrel == nullptr || Turret == nullptr)
		//return;
	
	if(TankMesh == nullptr)
		return;

	//vehicle 修改
	//FVector StartLocation = Barrel->GetSocketLocation(FName("FireLocation"));
	FVector StartLocation = TankMesh->GetSocketLocation(FName("gun_1_jntSocket"));
	//UE_LOG(LogTemp,Warning,TEXT("socket is %s"),*StartLocation.ToString());
	bool bHaveSolution= false;
	
	bHaveSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		FireVector,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace);

	if(bHaveSolution)
	{
		//转向
		//UE_LOG(LogTemp,Warning,TEXT("Fire Location is %s"),*FireVector.GetSafeNormal().ToString());
		DrawDebugLine(
				GetWorld(),
				StartLocation,
				FireVector * 100000.0f,
				FColor::Red,
				false,
				0.0f,
				0.0f,
				10.0f);

		SetFireVector(FireVector);
		//Turret->MoveTurret(FireVector.GetSafeNormal());
		//Barrel->MoveBarrel(FireVector.GetSafeNormal());
	}
}

void UTankAimingComponent::StartFire()
{
	//if(Barrel == nullptr || Turret == nullptr)
		//return;
	ATankVehicle* Tank = Cast<ATankVehicle>(GetOwner());
	EProjectileType ProjectileType = Tank->CurrentProject;
	switch (Tank->CurrentProject)
	{
	case  EProjectileType::AP:
		if(Tank->AttributeSet->GetAP_Count()<=0)
		{
			UE_LOG(LogTemp,Warning,TEXT("AP 没了"));
			//return;
		}
			
		break;
	case  EProjectileType::APCR:
		if(Tank->AttributeSet->GetAPCR_Count()<=0)
		{
			UE_LOG(LogTemp,Warning,TEXT("APCR 没了"));
			//return;
		}
			
		break;
	case EProjectileType::HE:
		if(Tank->AttributeSet->GetHE_Count()<=0)
		{
			UE_LOG(LogTemp,Warning,TEXT("HE 没了"));
			//return;
		}
		break;
	default:;
	}
	
	if(TankMesh == nullptr)
		return;
	if (!bIsFiringWeapon)
	{
		//UE_LOG(LogTemp,Warning,TEXT("start fire"));
		//bIsFiringWeapon = true;
		SetIsFiringWeapon(true);
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(FiringTimer, this, &UTankAimingComponent::StopFire, FireRate, false);
		//HandleFire();
	}
}

void UTankAimingComponent::StopFire()
{
	
	//bIsFiringWeapon = false;
	SetIsFiringWeapon(false);
	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::HandleFire()
{
	//FVector spawnLocation = Barrel->GetSocketLocation(FName("FireLocation"));
	//FRotator spawnRotation = Barrel->GetSocketRotation(FName("FireLocation"));


	//载具版本
	FVector spawnLocation = TankMesh->GetSocketLocation(FName("gun_1_jntSocket"));
	FRotator spawnRotation = TankMesh->GetSocketRotation(FName("gun_1_jntSocket"));

	//UE_LOG(LogTemp,Warning,TEXT("fire location %s"),*spawnLocation.ToString());
	FActorSpawnParameters spawnParameters;	
	spawnParameters.Instigator = GetOwner()->GetInstigator();
	spawnParameters.Owner = GetOwner();

	AProjectile* spawnedProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,spawnLocation,spawnRotation,spawnParameters);
	
	spawnedProjectile->LaunchProjectile(LaunchSpeed);
}

void UTankAimingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	/**需要复制的属性*/
	DOREPLIFETIME(UTankAimingComponent,bIsFiringWeapon);
	DOREPLIFETIME(UTankAimingComponent,FireVector);
}

void UTankAimingComponent::SetIsFiringWeapon_Implementation(bool FiringWeapon)
{
	if(Cast<APawn>(GetOwner())->GetLocalRole() == ROLE_Authority)
	{
		bIsFiringWeapon = FiringWeapon;
		OnIsFiringWeaponUpdate();
	}
}



void UTankAimingComponent::OnIsFiringWeaponUpdate()
{
	//UE_LOG(LogTemp,Warning,TEXT("Weapon update fire"));
	if(Cast<APawn>(GetOwner())->GetLocalRole() == ROLE_AutonomousProxy  || Cast<APawn>(GetOwner())->GetLocalRole() == ROLE_SimulatedProxy)
	{
		//UE_LOG(LogTemp,Warning,TEXT("weapon is %d"),bIsFiringWeapon);
		//客户端生成
		if(bIsFiringWeapon)
		{
			UE_LOG(LogTemp,Warning,TEXT("client Fire"));
			HandleFire();
			//开启技能

			
	
			//特效
			FTransform ParticleTransform = TankMesh->GetSocketTransform(FName("gun_1_jntSocket"));
 			FireImpactParticle->SetWorldTransform(ParticleTransform);
			SmokeImpactParticle->SetWorldTransform(ParticleTransform);
			
			FireImpactParticle->SetActive(true);
			SmokeImpactParticle->SetActive(true);
		}
		else
		{
			FTransform ParticleTransform = TankMesh->GetSocketTransform(FName("gun_1_jntSocket"));
			FireImpactParticle->SetWorldTransform(ParticleTransform);
			SmokeImpactParticle->SetWorldTransform(ParticleTransform);
			
			FireImpactParticle->SetActive(false);
			SmokeImpactParticle->SetActive(false);
		}
	}
	else if(Cast<ATankAIController>(Cast<APawn>(GetOwner())->GetController()))
	{
		
		if(bIsFiringWeapon)
		{
			UE_LOG(LogTemp,Warning,TEXT("AI fire"));
			HandleFire();
			ChangeWeaponAttribute();
		}
	}
	else if(Cast<APawn>(GetOwner())->GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp,Warning,TEXT("server fire"));
		if(bIsFiringWeapon)
		{
			HandleFire();
			//服务器上 播放技能
			//if(Cast<APawn>(GetOwner())->IsLocallyControlled())
			ChangeWeaponAttribute();

			
			FTransform ParticleTransform = TankMesh->GetSocketTransform(FName("gun_1_jntSocket"));
			FireImpactParticle->SetWorldTransform(ParticleTransform);
			SmokeImpactParticle->SetWorldTransform(ParticleTransform);
			
			FireImpactParticle->SetActive(true);
			SmokeImpactParticle->SetActive(true);
		}
		else
		{
			FTransform ParticleTransform = TankMesh->GetSocketTransform(FName("gun_1_jntSocket"));
			FireImpactParticle->SetWorldTransform(ParticleTransform);
			SmokeImpactParticle->SetWorldTransform(ParticleTransform);
			
			FireImpactParticle->SetActive(false);
			SmokeImpactParticle->SetActive(false);
		}
	}
	
}

void UTankAimingComponent::ChangeWeaponAttribute()
{
	//需要判断是否有子弹
	ATankVehicle* Tank =  Cast<ATankVehicle>(GetOwner());

	FGameplayTag ChangeCountTag;
	switch (Tank->CurrentProject)
	{
	case  EProjectileType::AP:
		UE_LOG(LogTemp,Warning,TEXT("Load fire AP"));
		ChangeCountTag = FGameplayTag::RequestGameplayTag(FName("Ability.ChangeDamage.AP")); 
		break;
	case EProjectileType::APCR:
		UE_LOG(LogTemp,Warning,TEXT("Load fire APCR"));
		ChangeCountTag = FGameplayTag::RequestGameplayTag(FName("Ability.ChangeDamage.APCR"));
		break;
	case EProjectileType::HE:
		UE_LOG(LogTemp,Warning,TEXT("Load fire HE"));
		ChangeCountTag = FGameplayTag::RequestGameplayTag(FName("Ability.ChangeDamage.HE"));
		break;
	default:;
	}

	FGameplayTagContainer ChangeCountTags;
	ChangeCountTags.AddTag(ChangeCountTag);
	
	if(Tank->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(ChangeCountTags))
	{
		UE_LOG(LogTemp,Warning,TEXT("fire projectile is %s  %d"),*ChangeCountTag.ToString(),ChangeCountTags.Num());
	}
	
	
	FGameplayTagContainer FireTags;
	FGameplayTag FireTag = FGameplayTag::RequestGameplayTag(FName("Ability.Hurt.Fire"));
	FireTags.AddTag(FireTag);
	Tank->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FireTags);
}



void UTankAimingComponent::OnRep_bIsFiringWeapon()
{
	UE_LOG(LogTemp,Warning,TEXT("fire change"));
	OnIsFiringWeaponUpdate();
}

void UTankAimingComponent::OnRep_FireVector()
{
	
	OnFireVectorUpdate();
}


FVector UTankAimingComponent::GetFireVector()
{
	return FireVector;
}

void UTankAimingComponent::SetFireVector_Implementation(FVector NewFireVector)
{
	FireVector = NewFireVector;
	//OnFireVectorUpdate();
}

void UTankAimingComponent::OnFireVectorUpdate()
{
	if(Cast<APawn>(GetOwner())->GetLocalRole() == ROLE_AutonomousProxy  || Cast<APawn>(GetOwner())->GetLocalRole() == ROLE_SimulatedProxy)
	{
		
		
	}
	if(Cast<APawn>(GetOwner())->GetLocalRole() == ROLE_Authority)
	{
		
	}
}



float UTankAimingComponent::GetChangeYaw()
{
	return this->ChangeYaw;
}






