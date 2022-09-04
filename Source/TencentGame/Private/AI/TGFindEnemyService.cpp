// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TGFindEnemyService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "TGUtils.h"
#include "TGAIPerceptionComponent.h"

UTGFindEnemyService::UTGFindEnemyService()
{
	NodeName = "Find Enemy";
}

void UTGFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (const auto Blackboard = OwnerComp.GetBlackboardComponent())
	{
		const auto Controller = OwnerComp.GetAIOwner();
		const auto PerceptionComponent = TGUtils::GetTGPlayerComponent<UTGAIPerceptionComponent>(Controller);
		if (PerceptionComponent)
		{
			Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
