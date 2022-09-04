// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "TGAIPerceptionComponent.generated.h"

/**
 * 
 */
UCLASS()
class TENCENTGAME_API UTGAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	AActor* GetClosestEnemy() const;
};
