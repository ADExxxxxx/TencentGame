// Fill out your copyright notice in the Description page of Project Settings.


#include "TGBaseWeapon.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "Components/SpotLightComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(BaseWeaponLog, All, All);

ATGBaseWeapon::ATGBaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);

	SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>("SpotLightComponent");
	SpotLightComponent->SetupAttachment(WeaponMesh);
}

void ATGBaseWeapon::OpenLight()
{
	if(IsOpen) SpotLightComponent->SetIntensity(0.0f);
	else SpotLightComponent->SetIntensity(5000.0f);
	IsOpen = !IsOpen;
	// SpotLightComponent->SetLightBrightness()
}

void ATGBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);
	check(SpotLightComponent);
	checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count could not be less or equal to zero!"));
	checkf(DefaultAmmo.Clips > 0, TEXT("Clips count could not be less or equal to zero!"));
	CurrentAmmo = DefaultAmmo;
}

void ATGBaseWeapon::StartFire()
{
	FireInProgress = true;
	UE_LOG(LogTemp, Error, TEXT("%d"), GetOwner()->GetLocalRole());
}

void ATGBaseWeapon::StopFire()
{
	FireInProgress = false;
}

bool ATGBaseWeapon::IsFiring() const
{
	return FireInProgress;
}

bool ATGBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
	if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0) return false;

	if (IsAmmoEmpty())
	{
		CurrentAmmo.Clips = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.Clips + 1);
		OnClipEmpty.Broadcast(this);
	}
	else if (CurrentAmmo.Clips < DefaultAmmo.Clips)
	{
		const auto NextClipsAmount = CurrentAmmo.Clips + ClipsAmount;
		if (DefaultAmmo.Clips - NextClipsAmount >= 0)
		{
			CurrentAmmo.Clips = NextClipsAmount;
		}
		else
		{
			CurrentAmmo.Clips = DefaultAmmo.Clips;
			CurrentAmmo.Bullets = DefaultAmmo.Bullets;
		}
	}
	else
	{
		CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	}

	return true;
}

bool ATGBaseWeapon::CanReload() const
{
	return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

void ATGBaseWeapon::MakeShot()
{
}

APlayerController* ATGBaseWeapon::GetPlayerController() const
{
	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player) return nullptr;

	return Player->GetController<APlayerController>();
}

bool ATGBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto TGCharacter = Cast<ACharacter>(GetOwner());
	if (!TGCharacter) return false;

	if (TGCharacter->IsPlayerControlled())
	{
		const auto Controller = TGCharacter->GetController<APlayerController>();
		if (!Controller) return false;

		Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}
	else
	{
		ViewLocation = GetMuzzleWorldLocation();
		ViewRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
	}
	return true;
}

FVector ATGBaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ATGBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	const FVector ShootDirection = ViewRotation.Vector();
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ATGBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
	if (!GetWorld()) return;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bReturnPhysicalMaterial = true;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility,
	                                     CollisionQueryParams);
}

void ATGBaseWeapon::DecreaseAmmo()
{
	if (CurrentAmmo.Bullets == 0) return;
	CurrentAmmo.Bullets--;

	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		StopFire();
		OnClipEmpty.Broadcast(this);
	}
}

bool ATGBaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ATGBaseWeapon::IsAmmoFull() const
{
	return CurrentAmmo.Clips == DefaultAmmo.Clips &&
		   CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}

bool ATGBaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets == 0;
}

void ATGBaseWeapon::ChangeClip()
{
	if (!CurrentAmmo.Infinite)
	{
		if (CurrentAmmo.Clips == 0) return;
		CurrentAmmo.Clips--;
	}
	CurrentAmmo.Bullets = DefaultAmmo.Bullets;
}

UNiagaraComponent* ATGBaseWeapon::SpawnMuzzleFX()
{
	return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX,  
		WeaponMesh,                                                
		MuzzleSocketName,                                          
		FVector::ZeroVector,                                       
		FRotator::ZeroRotator,                                     
		EAttachLocation::SnapToTarget, true);
}

void ATGBaseWeapon::LogAmmo()
{
	FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
	AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
	// UE_LOG(BaseWeaponLog, Display, TEXT("%s"), *AmmoInfo);
}
