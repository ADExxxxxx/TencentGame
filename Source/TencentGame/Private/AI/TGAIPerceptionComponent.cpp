// Fill out your copyright notice in the Description page of Project Settings.


#include "TGAIPerceptionComponent.h"

#include "AIController.h"
#include "TGAICharacter.h"
#include "TGHealthComponent.h"
#include "TGUtils.h"
#include "Perception/AISense_Sight.h"

AActor* UTGAIPerceptionComponent::GetClosestEnemy() const
{
	TArray<AActor*> PercieveActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);
	if(PercieveActors.Num() == 0) return nullptr;

	const auto Controller = Cast<AAIController>(GetOwner());
	if(!Controller) return nullptr;

	const auto Pawn = Controller->GetPawn();
	if(!Pawn) return nullptr;

	float BestDistance = MAX_FLT;
	AActor* BestPawn = nullptr;

	// 找出感知到距离最近的Actor
	for(const auto PercieveActor : PercieveActors)
	{
		const auto HealthComponent = TGUtils::GetTGPlayerComponent<UTGHealthComponent>(PercieveActor);
		
		if(HealthComponent && !HealthComponent->IsDead() && !(PercieveActor->StaticClass() == StaticClass()))
		{
			UE_LOG(LogTemp, Error, TEXT("wwwwww %d"), PercieveActor->StaticClass() == StaticClass());
			const auto CurrentDistance = (PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
			if(CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestPawn = PercieveActor;
			}
		}
	}
	return BestPawn;
}
