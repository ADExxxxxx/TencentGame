// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/TGAnimNotify.h"

void UTGAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotified.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);
}