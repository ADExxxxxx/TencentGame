#include "Components/TGWeaponComponent.h"
#include "Weapon/TGBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/TGEquipFinishedAnimNotify.h"
#include "Animations/TGReloadFinishedAnimNotify.h"
#include "Animations/AnimUtils.h"
#include "Net/UnrealNetwork.h"

UTGWeaponComponent::UTGWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UTGWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	if(CurrentWeapon)
	{
		UIData = CurrentWeapon->GetUIData();
		return true;
	}
	return false;
}

bool UTGWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	if(CurrentWeapon)
	{
		AmmoData = CurrentWeapon->GetAmmoData();
		return true;
	}
	return false;
}

void UTGWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentWeaponIndex = 0;
	InitAnimations();
	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
	SetIsReplicated(true);
}

void UTGWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;
	for (const auto Weapon : Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		Weapon->Destroy();
	}
	Weapons.Empty();
	Super::EndPlay(EndPlayReason);
}

void UTGWeaponComponent::SpawnWeapons()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || !GetWorld()) return;
	for (auto OneWeaponData : WeaponData)
	{
		auto Weapon = GetWorld()->SpawnActor<ATGBaseWeapon>(OneWeaponData.WeaponClass);
		if (!Weapon) continue;
		Weapon->OnClipEmpty.AddUObject(this, &UTGWeaponComponent::OnClipEmpty);
		Weapon->SetOwner(Character);
		Weapons.Add(Weapon);
		AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
	}
}

void UTGWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	if (WeaponIndex < 0 || WeaponIndex >= WeaponData.Num()) return;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
	}
	CurrentWeapon = Weapons[WeaponIndex];
	CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
	const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data)
	{
		return Data.WeaponClass == CurrentWeapon->GetClass();
	});
	CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;
	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
	EquipAnimInProgress = true;
	PlayAnimMontage(EquipAnimMontage);
}

void UTGWeaponComponent::AttachWeaponToSocket(ATGBaseWeapon* Weapon, USceneComponent* SceneComponent,
                                              const FName& SocketName)
{
	if (!Weapon || !SceneComponent) return;
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void UTGWeaponComponent::StartFire()
{
	if (!CanFire()) return;
	CurrentWeapon->StartFire();
}

void UTGWeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
}

void UTGWeaponComponent::NextWeapon()
{
	if (!CanEquip()) return;

	int32 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	while (NextIndex != CurrentWeaponIndex)
	{
		if (!Weapons[NextIndex]->IsAmmoEmpty()) break;
		NextIndex = (NextIndex + 1) % Weapons.Num();
	}

	if (CurrentWeaponIndex != NextIndex)
	{
		CurrentWeaponIndex = NextIndex;
		EquipWeapon(CurrentWeaponIndex);
	}
}

void UTGWeaponComponent::Reload()
{
	ChangeClip();
}

void UTGWeaponComponent::OpenLight()
{
	CurrentWeapon->OpenLight();
}

/*
 * Animation Event
 */
void UTGWeaponComponent::InitAnimations()
{
	auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<UTGEquipFinishedAnimNotify>(EquipAnimMontage);
	if (EquipFinishedNotify)
	{
		EquipFinishedNotify->OnNotified.AddUObject(this, &UTGWeaponComponent::OnEquipFinished);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Equip anim notify is forgotten to set"));
		checkNoEntry();
	}

	for(auto OneWeaponData : WeaponData)
	{
		auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<UTGReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
		if (!ReloadFinishedNotify)
		{
			UE_LOG(LogTemp, Error, TEXT("Reload anim notify is forgotten to set"));
			checkNoEntry();
		}
		ReloadFinishedNotify->OnNotified.AddUObject(this, &UTGWeaponComponent::OnReloadFinished);
	}
}

void UTGWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || MeshComponent != Character->GetMesh()) return;
	EquipAnimInProgress = false;
}

void UTGWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!Character || MeshComponent != Character->GetMesh()) return;
	ReloadAnimInProgress = false;
}

bool UTGWeaponComponent::CanEquip() const
{
	return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UTGWeaponComponent::CanReload() const
{
	return CurrentWeapon
	   && !EquipAnimInProgress
	   && !ReloadAnimInProgress
	   && CurrentWeapon->CanReload();
}

bool UTGWeaponComponent::CanFire() const
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

void UTGWeaponComponent::ChangeClip()
{
	if (!CanReload()) return;
	CurrentWeapon->StopFire();
	CurrentWeapon->ChangeClip();
	ReloadAnimInProgress = true;
	PlayAnimMontage(CurrentReloadAnimMontage);
}

void UTGWeaponComponent::OnClipEmpty(ATGBaseWeapon* ClipEmptyWeapon)
{
	if (!ClipEmptyWeapon) return;

	if (CurrentWeapon == ClipEmptyWeapon)
	{
		ChangeClip();
	}
	else
	{
		for (const auto Weapon : Weapons)
		{
			if (Weapon == ClipEmptyWeapon)
			{
				Weapon->ChangeClip();
			}
		}
	}
}

void UTGWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;
	Character->PlayAnimMontage(Animation);
}

//void UTGWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	DOREPLIFETIME(UTGWeaponComponent, CurrentWeapon);
//	DOREPLIFETIME(UTGWeaponComponent, CurrentWeaponIndex);
//	DOREPLIFETIME(UTGWeaponComponent, Weapons);
//}
