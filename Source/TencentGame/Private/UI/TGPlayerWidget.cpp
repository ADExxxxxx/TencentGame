// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TGPlayerWidget.h"
#include "TGUtils.h"
#include "TGHealthComponent.h"
#include "TGPlayerState.h"
#include "TGWeaponComponent.h"

float UTGPlayerWidget::GetHealthPercent() const
{
	
	const auto HealthComponent = TGUtils::GetTGPlayerComponent<UTGHealthComponent>(GetOwningPlayerPawn());
	if(!HealthComponent) return 0.0f;

	return HealthComponent->GetHealthPercent();
}

bool UTGPlayerWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	const auto WeaponComponent = TGUtils::GetTGPlayerComponent<UTGWeaponComponent>(GetOwningPlayerPawn());
	if(!WeaponComponent) return false;

	return WeaponComponent->GetCurrentWeaponUIData(UIData);
}

bool UTGPlayerWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{

	const auto WeaponComponent = TGUtils::GetTGPlayerComponent<UTGWeaponComponent>(GetOwningPlayerPawn());
	if(!WeaponComponent) return false;

	return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

bool UTGPlayerWidget::IsPlayerAlive() const
{
	const auto HealthComponent = TGUtils::GetTGPlayerComponent<UTGHealthComponent>(GetOwningPlayerPawn());
	return HealthComponent && !HealthComponent->IsDead();
	
}

bool UTGPlayerWidget::IsPlayerSpectating() const
{
	const auto Controller = GetOwningPlayer();
	return Controller && Controller->GetStateName() == NAME_Spectating;
}

int32 UTGPlayerWidget::GetKillsNum() const
{
	const auto Controller = GetOwningPlayer();
	if (!Controller) return 0;

	const auto PlayerState = Cast<ATGPlayerState>(Controller->PlayerState);
	return PlayerState ? PlayerState->GetKillsNum() : 0;
}
