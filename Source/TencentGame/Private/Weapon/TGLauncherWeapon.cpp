// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/TGLauncherWeapon.h"

#include "DrawDebugHelpers.h"
#include "Weapon/TGProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ATGLauncherWeapon::StartFire()
{
	Super::StartFire();
	MakeShot();
	// DrawPreviewLine(GetMuzzleWorldLocation(), FVector(2000.0f, 0.0f, 0.0f), 0.0f, 20);
	
}

void ATGLauncherWeapon::StopFire()
{
	Super::StopFire();
}

void ATGLauncherWeapon::MakeShot()
{
	if (!GetWorld() || IsAmmoEmpty()) return;
	
	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) return;

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

	const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
	ATGProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATGProjectile>(ProjectileClass, SpawnTransform);
	if (Projectile)
	{
		Projectile->SetShotDirection(Direction);
		Projectile->SetOwner(GetOwner());
		Projectile->FinishSpawning(SpawnTransform);
	}
	DecreaseAmmo();
	SpawnMuzzleFX();
	UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);

	StopFire();
}

// void ATGLauncherWeapon::ServerMakeShot_Implementation()
// {
// 	MakeShot();
// }


void ATGLauncherWeapon::DrawPreviewLine(const FVector& FromLocation, const FVector& Velocity, float Gravity, int N)
{
	const float ZAxisVelocity = Velocity.Z;
	const FVector2D XYAxisVelocity = FVector2D(Velocity.X, Velocity.Y);
	constexpr float Dt = 0.05f;
	for (float t = 0; t <= N * Dt; t += Dt)
	{
		bool End = 0;
		const float Z0 = ZAxisVelocity * t - 0.5 * Gravity * t * t;
		const FVector2D XY0 = XYAxisVelocity * t;
		const float Z1 = ZAxisVelocity * (t + Dt) - 0.5 * Gravity * (t + Dt) * (t + Dt);
		const FVector2D XY1 = XYAxisVelocity * (t + Dt);
		FVector Point0 = FVector(XY0, Z0) + FromLocation;
		FVector Point1 = FVector(XY1, Z1) + FromLocation;
		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, Point0, Point1, ECC_Visibility);
		if (HitResult.GetActor())
		{
			End = true;
			Point1 = HitResult.Location;
			GEngine->AddOnScreenDebugMessage(2, 3.0f, FColor::Blue, HitResult.ToString());
		}
		DrawDebugLine(GetWorld(), Point0, Point1, FColor::Red, false, -1, 0, 3);
		if (End)
		{
			DrawDebugSphere(GetWorld(), Point1, 5.0f, 10, FColor::Blue);
			break;
		}
	}
}
