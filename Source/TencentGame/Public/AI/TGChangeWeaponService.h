// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TGChangeWeaponService.generated.h"

/**
 * 
 */
UCLASS()
class TENCENTGAME_API UTGChangeWeaponService : public UBTService
{
	GENERATED_BODY()
public:
	UTGChangeWeaponService();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Probability = 0.5f;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
