// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TGCoreTypes.h"
#include "TGHealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TENCENTGAME_API UTGHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTGHealthComponent();
	float GetHealth() const { return Health;}

	UFUNCTION(BlueprintCallable, Category= "Health")
	bool IsDead() const { return FMath::IsNearlyZero(Health); }

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercent() const {return Health / MaxHealth;}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta=(ClampMin = "0.0", ClampMax = "200.0"))
	bool bAutoHeal = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta=(ClampMin = "0.0", ClampMax = "200.0"))
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	float HealUpdateTime = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	float HealDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	float HealModifier = 5.0f;
	
private:
	UPROPERTY(Replicated)
	float Health = 0.0f;
	
	FTimerHandle HealTimerHandle;
	
	void Killed(AController* KillerController);
	
	UFUNCTION()
	void OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	void ApplyDamage(float Damage, AController* InstigatedBy);
	void HealUpdate();
	void SetHealth(float NewHealth);
};
