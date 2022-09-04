// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TGChangeWeaponService.h"
#include "Components/TGWeaponComponent.h"
#include "AIController.h"
#include "TGUtils.h"

UTGChangeWeaponService::UTGChangeWeaponService()
{
	NodeName = "Change Weapon";
}

void UTGChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (const auto Controller = OwnerComp.GetAIOwner())
	{
		const auto WeaponComponent = TGUtils::GetTGPlayerComponent<UTGWeaponComponent>(Controller->GetPawn());
		if (WeaponComponent && Probability > 0 && FMath::FRand() <= Probability)
		{
			WeaponComponent->NextWeapon();
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}