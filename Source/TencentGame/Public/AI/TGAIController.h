// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TGAIPerceptionComponent.h"
#include "TGAIController.generated.h"

/**
 * 
 */
UCLASS()
class TENCENTGAME_API ATGAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATGAIController();
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UTGAIPerceptionComponent* TGPerceptionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName FocusOnKeyName = "EnemyActor";
	
	virtual void OnPossess(APawn* InPawn) override;
	
	virtual void Tick(float DeltaTime) override;

	
private:
	AActor* GetFocusOnActor() const;
};
