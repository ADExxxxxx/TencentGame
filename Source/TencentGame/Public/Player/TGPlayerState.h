// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TGPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TENCENTGAME_API ATGPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	void AddKill() { ++KillsNum; }
	
	UFUNCTION(BlueprintCallable)
	int32 GetKillsNum() const { return KillsNum; }
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 KillsNum = 0;

	
};
