// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/TGBaseWeapon.h"
#include "TGRifleWeapon.generated.h"


USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()
	
	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY()
	FHitResult HitResult;

	UPROPERTY()
	FVector_NetQuantize TraceEnd;
};

UCLASS()
class TENCENTGAME_API ATGRifleWeapon : public ATGBaseWeapon
{
	GENERATED_BODY()
public:
	virtual void StartFire() override;
	virtual void StopFire() override;
	ATGRifleWeapon();

	// void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TimeBetweenShots = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float BulletSpread = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float DamageAmount = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* TraceFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	FString TraceTargetName = "TraceTarget";

	UPROPERTY(VisibleAnywhere, Category = "VFX")
	class UTGWeaponFXComponent* WeaponFXComponent;
                                               
	virtual void BeginPlay() override;               
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

	virtual void MakeShot() override;

	// UFUNCTION(Server, Reliable)
	// void ServerMakeShot();

	// UFUNCTION(NetMulticast, Reliable)
	// void MultiMakeShot();
	
	// UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)
	// FHitScanTrace HitScanTrace;
// // 
	// UFUNCTION()
	// void OnRep_HitScanTrace();

	

private:
	FTimerHandle ShotTimerHandle;

	UPROPERTY()
	class UNiagaraComponent* MuzzleFXComponent;

	UPROPERTY()
	UAudioComponent* FireAudioComponent;
	
	void MakeDamage(const FHitResult& HitResult);
	void InitFX();
	void SetFXActive(bool IsActive);
	void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);
};
