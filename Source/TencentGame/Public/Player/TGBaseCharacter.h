// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TGHealthComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TGBaseCharacter.generated.h"

UCLASS()
class TENCENTGAME_API ATGBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATGBaseCharacter(const FObjectInitializer& ObjectInit);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Commponents")
	UTGHealthComponent *HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Commponents")
	class UTextRenderComponent *HealthTextRenderComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Commponents")
	class UTGWeaponComponent *WeaponComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage *DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float LifeSpanOnDeath = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category= "Damage")
	FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandedDamage = FVector2D(10.0f, 100.0f);
	
	virtual void BeginPlay() override;
	virtual void OnHealthChanged(float Health);
	virtual void OnDeath();

public:	
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;
	
private:
	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);
};
