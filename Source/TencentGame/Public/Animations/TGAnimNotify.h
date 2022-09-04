// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "TGAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifyedSignature, USkeletalMeshComponent*)
/**
 * 
 */
UCLASS()
class TENCENTGAME_API UTGAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual  void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	FOnNotifyedSignature OnNotified;
	
};
