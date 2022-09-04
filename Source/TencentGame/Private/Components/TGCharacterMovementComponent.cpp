// Fill out your copyright notice in the Description page of Project Settings.


#include "TGCharacterMovementComponent.h"
#include "Player/TGBaseCharacter.h"

float UTGCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	const ATGBaseCharacter* Player = Cast<ATGBaseCharacter>(GetPawnOwner());
	return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}
