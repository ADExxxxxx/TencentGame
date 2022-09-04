// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/TGFireService.h"
#include "AIController.h"

#include "TGUtils.h"
#include "Components/TGWeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UTGFireService::UTGFireService()
{
	NodeName = "Fire";
}

void UTGFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	const auto HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

	if (const auto Controller = OwnerComp.GetAIOwner())
	{
		if (const auto WeaponComponent = TGUtils::GetTGPlayerComponent<UTGWeaponComponent>(Controller->GetPawn()))
		{
			// UE_LOG(LogTemp, Error, TEXT("Fire %i"), HasAim);
			HasAim ? WeaponComponent->StartFire() : WeaponComponent->StopFire();
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
