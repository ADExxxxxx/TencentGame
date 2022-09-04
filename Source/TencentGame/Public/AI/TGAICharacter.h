// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Player/TGBaseCharacter.h"
#include "TGAICharacter.generated.h"

/**
 * 
 */
UCLASS()
class TENCENTGAME_API ATGAICharacter : public ATGBaseCharacter
{
	GENERATED_BODY()
public:
	ATGAICharacter(const FObjectInitializer& ObjectInit);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void OnDeath() override;
	virtual void OnHealthChanged(float Health) override;
};
