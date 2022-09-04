// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TGPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TGWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"

ATGPlayerCharacter::ATGPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    PrimaryActorTick.bCanEverTick = true;

    // 添加组件
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);
	
    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
	
    CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
    CameraCollisionComponent->SetupAttachment(CameraComponent);
    CameraCollisionComponent->SetSphereRadius(10.0f);
    CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    CameraCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ATGPlayerCharacter::OnDeath()
{
	Super::OnDeath();
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
		
	}
}

void ATGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(CameraCollisionComponent);

	CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATGPlayerCharacter::OnCameraCollisionBeginOverlap);
	CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ATGPlayerCharacter::OnCameraCollisionEndOverlap);

}

bool ATGPlayerCharacter::IsRunning() const
{
	return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

void ATGPlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckCameraOverlap();
}

void ATGPlayerCharacter::OnCameraCollisionEndOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckCameraOverlap();
}

void ATGPlayerCharacter::CheckCameraOverlap() const
{
	const auto HideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
	GetMesh()->SetOwnerNoSee(HideMesh);

	TArray<USceneComponent*> MeshChildren;
	GetMesh()->GetChildrenComponents(true, MeshChildren);

	for (auto MeshChild : MeshChildren)
	{
		if (const auto MeshChildGeometry = Cast<UPrimitiveComponent>(MeshChild))
		{
			MeshChildGeometry->SetOwnerNoSee(HideMesh);
		}
	}
}

void ATGPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	check(WeaponComponent);

	// 用户控制
	PlayerInputComponent->BindAxis("MoveForward", this, &ATGPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATGPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ATGPlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround", this, &ATGPlayerCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATGPlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ATGPlayerCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ATGPlayerCharacter::OnStopRunning);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UTGWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UTGWeaponComponent::StopFire);
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UTGWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UTGWeaponComponent::Reload);
	PlayerInputComponent->BindAction("OpenLight", IE_Pressed, WeaponComponent, &UTGWeaponComponent::OpenLight);


	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ATGPlayerCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Repeat, this, &ATGPlayerCharacter::TouchMoved);
}

void ATGPlayerCharacter::MoveForward(float Amount)
{
	IsMovingForward = Amount > 0.0f;
	if(Amount == 0.0f) return;
	AddMovementInput(GetActorForwardVector(), Amount);
}

void ATGPlayerCharacter::MoveRight(float Amount)
{
	if(Amount == 0.0f) return;
	AddMovementInput(GetActorRightVector(), Amount);
}

void ATGPlayerCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	InitVector = FVector2D(Location.X, Location.Y);
}

void ATGPlayerCharacter::TouchMoved(ETouchIndex::Type FingerIndex, FVector Location)
{
	const FVector2D CurrentVector = FVector2D(Location.X, Location.Y);
	// TODO: 后期看看是否有性能问题
	FVector2D ViewportSize; 
	GetWorld()->GetGameViewport()->GetViewportSize(ViewportSize);
	const FVector2D Offset = InitVector - CurrentVector;
	const float YawOffSet = -(Offset.X * Intensity) / ViewportSize.X * BaseTurnRate;
	float PitchOffset = (Offset.Y * Intensity) / ViewportSize.Y * BaseLookUpRate;
	PitchOffset = -FMath::Clamp(PitchOffset, -89.0f, 89.0f);
	AddControllerYawInput(YawOffSet);
	AddControllerPitchInput(PitchOffset);
	InitVector = CurrentVector;
}

void ATGPlayerCharacter::OnStartRunning()
{
	WantsToRun = true;
}

void ATGPlayerCharacter::OnStopRunning()
{
	WantsToRun = false;
}
