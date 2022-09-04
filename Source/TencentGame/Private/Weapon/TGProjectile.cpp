#include "Weapon/TGProjectile.h"
#include "DrawDebugHelpers.h"
#include "TGWeaponFXComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ATGProjectile::ATGProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	SetRootComponent(CollisionComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->InitialSpeed = 2000.0f;
	MovementComponent->ProjectileGravityScale = 0.0f;

	WeaponFXComponent = CreateDefaultSubobject<UTGWeaponFXComponent>("WeaponFXComponent");
}

void ATGProjectile::BeginPlay()
{
	Super::BeginPlay();

	check(MovementComponent);
	check(CollisionComponent);
	check(WeaponFXComponent);
	
	MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	CollisionComponent->OnComponentHit.AddDynamic(this, &ATGProjectile::OnProjectileHit);
	SetLifeSpan(LifeSeconds);
}

void ATGProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetWorld()) return;
	MovementComponent->StopMovementImmediately();
	FCollisionShape ColSphere = FCollisionShape::MakeSphere(DamageRadius);
	

	TArray<FHitResult> OutHits;

	// Make Damage
	UGameplayStatics::ApplyRadialDamage(GetWorld(),
	                                    DamageAmount,
	                                    GetActorLocation(),
	                                    DamageRadius,
	                                    UDamageType::StaticClass(),
	                                    {GetOwner()},
	                                    this,
	                                    GetController(),
	                                    DoFullDamage);
	// DrawDebugSphere(GetWorld(), GetActorLocation(), ColSphere.GetSphereRadius(), 50, FColor::Cyan, true);
	bool IsHit = GetWorld()->SweepMultiByChannel(OutHits, GetActorLocation(), GetActorLocation(), FQuat::Identity, ECC_PhysicsBody, ColSphere);
	if (IsHit)
	{
		// loop through TArray
		for (auto& HitActor : OutHits)
		{
			const auto MeshComp = HitActor.GetComponent();
 
			if (MeshComp && MeshComp->IsSimulatingPhysics())
			{
				// alternivly you can use  ERadialImpulseFalloff::RIF_Linear for the impulse to get linearly weaker as it gets further from origin.
				// set the float radius to 500 and the float strength to 2000.
				MeshComp->AddRadialImpulse(GetActorLocation(), DamageRadius, 2000.f, ERadialImpulseFalloff::RIF_Constant, true);
			}
		}
	}
	// DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.0f);
	// AddRad
	WeaponFXComponent->PlayImpactFX(Hit);
	Destroy();
}

AController* ATGProjectile::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}
