// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/TGBaseCharacter.h"
#include "TGPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TENCENTGAME_API ATGPlayerCharacter : public ATGBaseCharacter
{
	GENERATED_BODY()

public:
	ATGPlayerCharacter(const FObjectInitializer& ObjInit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "View")
	float Intensity = 1.5f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "View")
	float BaseTurnRate = 45.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "View")
	float BaseLookUpRate = 45.0f;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual bool IsRunning()  const override;
	
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UCameraComponent *CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Commponents")
	class USpringArmComponent *SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Commponents")
	class USphereComponent *CameraCollisionComponent;

	virtual void OnDeath() override;
	virtual void BeginPlay() override;
	
private:
	bool WantsToRun = false;
	bool IsMovingForward = false;
	FVector2D InitVector;

	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void TouchMoved(ETouchIndex::Type FingerIndex, FVector Location);
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	UFUNCTION()
	void OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCameraCollisionEndOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CheckCameraOverlap() const;

	UFUNCTION(BlueprintCallable)
	void OnStartRunning();

	UFUNCTION(BlueprintCallable)
	void OnStopRunning();
};
