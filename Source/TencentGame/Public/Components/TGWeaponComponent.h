// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "TGBaseWeapon.h"
#include "TGCoreTypes.h"
#include "Components/ActorComponent.h"
#include "TGWeaponComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TENCENTGAME_API UTGWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTGWeaponComponent();

	UFUNCTION(BlueprintCallable)
	virtual void StartFire();

	UFUNCTION(BlueprintCallable)
	virtual void StopFire();

	UFUNCTION(BlueprintCallable)
	virtual void NextWeapon();

	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintCallable)
	void OpenLight();

	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;
	// virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void EquipWeapon(int32 WeaponIndex);
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<FWeaponData> WeaponData;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponEquipSocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponArmorySocketName = "ArmorySocket";

	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UAnimMontage* EquipAnimMontage;
	
	bool CanFire() const;
	bool CanEquip() const;
	bool CanReload() const;

	UPROPERTY()
	ATGBaseWeapon* CurrentWeapon = nullptr;

	UPROPERTY()
	TArray<ATGBaseWeapon*> Weapons;

	UPROPERTY()
	int32 CurrentWeaponIndex = 0;

private:
	
	UPROPERTY()
	UAnimMontage* CurrentReloadAnimMontage = nullptr;

	bool EquipAnimInProgress = false;
	bool ReloadAnimInProgress = false;
	
	
	
	void AttachWeaponToSocket(ATGBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
	void PlayAnimMontage(UAnimMontage* Animation);
	void InitAnimations();
	void SpawnWeapons();
	
	void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
	void OnReloadFinished(USkeletalMeshComponent* MeshComponent);
	
	

	void OnClipEmpty(ATGBaseWeapon* ClipEmptyWeapon);
	void ChangeClip();
};
