// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TGAnimNotify.h"
#include "TGEquipFinishedAnimNotify.generated.h"
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifyedSignature, USkeletalMeshComponent*)
/**
 * 
 */
UCLASS()
class TENCENTGAME_API UTGEquipFinishedAnimNotify : public UTGAnimNotify
{
	GENERATED_BODY()
};
