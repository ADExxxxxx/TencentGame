// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TGCoreTypes.h"

#include "TGBaseWeapon.generated.h"


UCLASS()
class TENCENTGAME_API ATGBaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ATGBaseWeapon();
	FOnClipEmptySignature OnClipEmpty;
	
	virtual void StartFire();
	virtual void StopFire();
	bool IsFiring() const;

	void ChangeClip();
	bool CanReload() const;
	void OpenLight();

	FWeaponUIData GetUIData() const {return UIData;}
	FAmmoData GetAmmoData() const {return CurrentAmmo;}
	
	bool TryToAddAmmo(int32 ClipsAmount);
	bool IsAmmoEmpty() const;
	bool IsAmmoFull() const;
	
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USkeletalMeshComponent* WeaponMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USpotLightComponent* SpotLightComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName MuzzleSocketName = "Muzzle";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TraceMaxDistance = 1500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FAmmoData DefaultAmmo{15, 10, false};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FWeaponUIData UIData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	class UNiagaraSystem* MuzzleFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	class USoundCue* FireSound;
	
	bool FireInProgress = false;
	
	virtual void BeginPlay() override;
	virtual void MakeShot();
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

	
	
	void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);
	APlayerController* GetPlayerController() const;
	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
	FVector GetMuzzleWorldLocation() const;
	void DecreaseAmmo();
	void LogAmmo();
	bool IsClipEmpty() const;

	class UNiagaraComponent* SpawnMuzzleFX();
	
private:
	FAmmoData CurrentAmmo;
	bool IsOpen = false;
};

