
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/TGBaseWeapon.h"
#include "TGGrenadeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TENCENTGAME_API ATGGrenadeWeapon : public ATGBaseWeapon
{
	GENERATED_BODY()
public:
	virtual void StartFire() override;
	virtual void StopFire() override;


protected:
	

	virtual void MakeShot() override;
};
