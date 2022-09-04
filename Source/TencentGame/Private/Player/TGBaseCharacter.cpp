// Fill out your copyright notice in the Description page of Project Settings.


#include "TGBaseCharacter.h"
#include "TGBaseWeapon.h"
#include "TGWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TGCharacterMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharaterLog, All, All);

ATGBaseCharacter::ATGBaseCharacter(const FObjectInitializer& ObjectInit)
	: Super(ObjectInit.SetDefaultSubobjectClass<UTGCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	
	PrimaryActorTick.bCanEverTick = true;
	HealthTextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>("TextRenderComponent");
	HealthTextRenderComponent->SetupAttachment(GetRootComponent());
	HealthTextRenderComponent->SetOwnerNoSee(true);
	HealthComponent = CreateDefaultSubobject<UTGHealthComponent>("HealthComponent");
	WeaponComponent = CreateDefaultSubobject<UTGWeaponComponent>("WeaponComponent");
}

void ATGBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(HealthComponent);
	check(WeaponComponent);
	check(GetCharacterMovement());
	check(GetCapsuleComponent());
	check(GetMesh());

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this, &ATGBaseCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ATGBaseCharacter::OnHealthChanged);

	LandedDelegate.AddDynamic(this, &ATGBaseCharacter::OnGroundLanded);
}

void ATGBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ATGBaseCharacter::IsRunning() const
{
	return false;
}

float ATGBaseCharacter::GetMovementDirection() const
{
	if(GetVelocity().IsZero()) return 0.0f;
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const auto Degree = FMath::RadiansToDegrees(AngleBetween);
	return  CrossProduct.IsZero() ? Degree : Degree * FMath::Sign(CrossProduct.Z);
}

void ATGBaseCharacter::OnDeath()
{
	PlayAnimMontage(DeathAnimMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(LifeSpanOnDeath);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	auto PlayerController =  Cast<APlayerController>(Controller);
	if(PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
	}
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
	WeaponComponent->StopFire();
}

void ATGBaseCharacter::OnHealthChanged(float Health)
{
	HealthTextRenderComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ATGBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
	const auto FallVelocityZ = -GetCharacterMovement()->Velocity.Z;

	if(FallVelocityZ < LandedDamageVelocity.X) return;
	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);

	TakeDamage(FinalDamage, FDamageEvent {}, nullptr, nullptr);
}

