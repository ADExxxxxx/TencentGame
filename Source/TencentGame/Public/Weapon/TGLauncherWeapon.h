// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/TGBaseWeapon.h"
#include "TGLauncherWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TENCENTGAME_API ATGLauncherWeapon : public ATGBaseWeapon
{
	GENERATED_BODY()
public:
	virtual void StartFire() override;
	virtual void StopFire() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class ATGProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* NoAmmoSound;
	
	virtual void MakeShot() override;

	// UFUNCTION(Server, Reliable)
	// void ServerMakeShot();

	void DrawPreviewLine(const FVector& FromLocation, const FVector& Velocity, float Gravity, int N);
	
};
