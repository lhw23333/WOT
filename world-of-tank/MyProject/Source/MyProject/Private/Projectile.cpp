// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "Abilities/GameplayAbilityTypes.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//bReplicates = true;

	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));
	ProjectileMovementComponent->bAutoActivate = false;
	//定义将作为投射物及其碰撞的根组件的SphereComponent。
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	SphereComponent->SetSimulatePhysics(false);
	SphereComponent->SetNotifyRigidBodyCollision(true);
	RootComponent = SphereComponent;
	//在击中事件上注册此投射物撞击函数。
	if (GetLocalRole() == ROLE_Authority)
	{
		SphereComponent->OnComponentHit.AddDynamic(this, &AProjectile::AProjectile::OnProjectileImpact);
	}
	
	//定义将作为视觉呈现的网格体。
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));


	//若成功找到要使用的静态网格体资产，则设置该静态网格体及其位置/比例。
	if (DefaultMesh.Succeeded())
	{
		StaticMesh->SetStaticMesh(DefaultMesh.Object);
		StaticMesh->GetRelativeLocation() = FVector(0.0f, 0.0f, -37.5f);
		StaticMesh->GetRelativeScale3D() = FVector(0.75f, 0.75f, 0.75f);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> DefaultExplosionEffect(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	if (DefaultExplosionEffect.Succeeded())
	{
		ExplosionEffect = DefaultExplosionEffect.Object;
	}
	
	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>("ExplosionForce");
	ExplosionForce->AttachTo(RootComponent);
	ExplosionForce->SetAutoActivate(false);


	DamageType = UDamageType::StaticClass();
	ProjectileDamage = 20.0f;
}

void AProjectile::SetProjectileParent(ATank* Parent)
{
	ProjectileParent = Parent;
}


// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::LaunchProjectile(float Speed)
{
	ProjectileMovementComponent->SetVelocityInLocalSpace((FVector::ForwardVector * Speed));

	ProjectileMovementComponent->Activate(true);
}

void AProjectile::OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ( OtherActor )
	{
		if(GetLocalRole() == ROLE_Authority)
		{
			//UE_LOG(LogTemp,Warning ,TEXT("other name %s"),*OtherActor->GetName());
			//发送Gameplay事件，GAS中实现damage效果
			//GAS damage
			
			//TakeDamage
			UGameplayStatics::ApplyPointDamage(OtherActor, ProjectileDamage, NormalImpulse, Hit, GetInstigator()->Controller, this, DamageType);
		}
			
	}
	//Destroy();
}

void AProjectile::Destroyed()
{
	
}


