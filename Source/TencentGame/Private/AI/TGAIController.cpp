// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TGAIController.h"
#include "AI/TGAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

ATGAIController::ATGAIController()
{
	TGPerceptionComponent = CreateDefaultSubobject<UTGAIPerceptionComponent>("PerceptionComponent");
	SetPerceptionComponent(*TGPerceptionComponent);
}


void ATGAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if(const auto TGCharacter = Cast<ATGAICharacter>(InPawn))
	{
		RunBehaviorTree(TGCharacter->BehaviorTreeAsset);
	}
}

void ATGAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetFocus(GetFocusOnActor());
}

AActor* ATGAIController::GetFocusOnActor() const
{
	if (!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
