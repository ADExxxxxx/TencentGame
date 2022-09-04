// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TGProjectile.generated.h"

UCLASS()
class TENCENTGAME_API ATGProjectile : public AActor
{
	GENERATED_BODY()

public:
	ATGProjectile();

	void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }

	UFUNCTION(BlueprintCallable)
	FVector GetDirection() { return ShotDirection; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	class UProjectileMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "VFX")
	class UTGWeaponFXComponent* WeaponFXComponent;

	// 爆炸径向半径
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	float DamageRadius = 200.0f;

	// 爆炸径向伤害
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	float DamageAmount = 50.0f;

	// 伤害扩散方式
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	bool DoFullDamage = false;

	// 生命时长
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	float LifeSeconds = 5.0f;
private:
	FVector ShotDirection;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	                     UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	AController* GetController() const;
};
