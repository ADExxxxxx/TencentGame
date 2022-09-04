// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TGAICharacter.h"

#include "TGAIWeaponComponent.h"
#include "TGCharacterMovementComponent.h"
#include "AI/TGAIController.h"

ATGAICharacter::ATGAICharacter(const FObjectInitializer& ObjectInit)
	:Super(ObjectInit.SetDefaultSubobjectClass<UTGAIWeaponComponent>("WeaponComponent"))
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = ATGAIController::StaticClass();

	bUseControllerRotationYaw = false;
	if(GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
	}
}

void ATGAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ATGAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATGAICharacter::OnHealthChanged(float Health)
{
	Super::OnHealthChanged(Health);
}


void ATGAICharacter::OnDeath()
{
	Super::OnDeath();

	const auto TGController = Cast<AAIController>(Controller);
	if (TGController && TGController->BrainComponent)
	{
		TGController->BrainComponent->Cleanup();
	}
}
