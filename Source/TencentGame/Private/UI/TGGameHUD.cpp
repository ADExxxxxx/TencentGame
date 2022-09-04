// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TGGameHUD.h"

#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"

void ATGGameHUD::DrawHUD()
{
	Super::DrawHUD();
}

void ATGGameHUD::BeginPlay()
{
	Super::BeginPlay();
	auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
	if(PlayerHUDWidget)
	{
		PlayerHUDWidget->AddToViewport();
	}
}

void ATGGameHUD::DrawCrossHair()
{
	const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

	const float HalfLineSize = 10.0f;
	const float LineThickness = 2.0f;
	const FLinearColor LinearColor = FLinearColor::Green;
	DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LinearColor, LineThickness);
	DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LinearColor, LineThickness);
}
