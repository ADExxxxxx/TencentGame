// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/TGRifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/TGWeaponFXComponent.h"
// #include "Net/UnrealNetwork.h"

ATGRifleWeapon::ATGRifleWeapon()
{
	WeaponFXComponent = CreateDefaultSubobject<UTGWeaponFXComponent>("WeaponFXComponent");
	SetReplicates(true);

	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;
}

void ATGRifleWeapon::StartFire()
{
	Super::StartFire();
	
	InitFX();
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ATGRifleWeapon::MakeShot, TimeBetweenShots, true);
	MakeShot();
}

void ATGRifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
	SetFXActive(false);
	Super::StopFire();
}

void ATGRifleWeapon::MakeShot()
{
	// if(GetOwner()->GetLocalRole() < ROLE_Authority)
	// {
	// 	ServerMakeShot();
	// }
	if(!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}
	FVector TraceStart, TraceEnd;
	if(!GetTraceData(TraceStart, TraceEnd))
	{
		StopFire();
		return;
	}

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);
	FVector TraceFXEnd = TraceEnd;
	// if(GetOwner()->GetLocalRole() == ROLE_Authority)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("Change"));
	// 	HitScanTrace.HitResult = HitResult;
	// 	HitScanTrace.TraceEnd = TraceFXEnd;
	// }
	if(HitResult.bBlockingHit)
	{
		TraceFXEnd = HitResult.ImpactPoint;
		const auto OtherComp = HitResult.GetComponent();
		if(OtherComp->IsSimulatingPhysics())
		{
			HitResult.GetComponent()->AddImpulseAtLocation((TraceEnd - TraceStart).GetSafeNormal() * 50000.0f, GetActorLocation());
		}
		// Todo: standAlone
		MakeDamage(HitResult);
		WeaponFXComponent->PlayImpactFX(HitResult);
		
		// Todo: 击中效果，之后需要加冲击力
		// DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
		// DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
		// HitResult
	}
	SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);
	DecreaseAmmo();
}

// void ATGRifleWeapon::ServerMakeShot_Implementation()
// {
// 	MakeShot();
// }

void ATGRifleWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponFXComponent);
}

bool ATGRifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if(!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	// 子弹在锥形区域随机偏移
	TraceStart = ViewLocation;
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ATGRifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamageActor = HitResult.GetActor();
	// UE_LOG(LogTemp, Display, TEXT("hit %s"), *DamageActor->GetName());
	if(!DamageActor) return;


	DamageActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}

void ATGRifleWeapon::InitFX()
{
	if (!MuzzleFXComponent)
	{
		MuzzleFXComponent = SpawnMuzzleFX();
	}

	if (!FireAudioComponent)
	{
		FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
	}
	SetFXActive(true);
}

void ATGRifleWeapon::SetFXActive(bool IsActive)
{
	if (MuzzleFXComponent)
	{
		MuzzleFXComponent->SetPaused(!IsActive);
		MuzzleFXComponent->SetVisibility(IsActive, true);
	}

	if (FireAudioComponent)
	{
		FireAudioComponent->SetPaused(!IsActive);
	}
}

void ATGRifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
	const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
	if (TraceFXComponent)
	{
		TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
	}
}

// void ATGRifleWeapon::OnRep_HitScanTrace()
// {
// 	UE_LOG(LogTemp, Error, TEXT("广播"));
// 	if(HitScanTrace.HitResult.bBlockingHit)
// 	{
// 		WeaponFXComponent->PlayImpactFX(HitScanTrace.HitResult);
// 	}
// 	SpawnTraceFX(GetMuzzleWorldLocation(), HitScanTrace.TraceEnd);
// }

// void ATGRifleWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)
// {
// 	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
// 
// 	DOREPLIFETIME_CONDITION(ATGRifleWeapon, HitScanTrace ,COND_SkipOwner);
// }