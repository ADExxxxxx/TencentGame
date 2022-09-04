#include "TGHealthComponent.h"
#include "TGGameModeBase.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(HealthComponentLog, All, All);

UTGHealthComponent::UTGHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTGHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	SetHealth(MaxHealth);
	check(MaxHealth > 0);
	
	
	AActor* ComponentOwner = GetOwner();
	if(ComponentOwner && GetOwnerRole() == ROLE_Authority)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UTGHealthComponent::OnTakeAnyDamageHandle);
	}
	SetIsReplicated(true);
}

void UTGHealthComponent::Killed(AController* KillerController)
{
	if (!GetWorld()) return;

	const auto GameMode = Cast<ATGGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;

	const auto Player = Cast<APawn>(GetOwner());
	const auto VictimController = Player ? Player->Controller : nullptr;

	GameMode->Killed(KillerController, VictimController);
}

void UTGHealthComponent::OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	
	if (Damage <= 0.0f || IsDead() || !GetWorld()) return;
	// UE_LOG(HealthComponentLog, Display, TEXT("%f"), Health);
	SetHealth(Health - Damage);
	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

	if (IsDead())
	{
		Killed(InstigatedBy);
		OnDeath.Broadcast();
	}
	else if (bAutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UTGHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}
}

void UTGHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy)
{
	if (Damage <= 0.0f || IsDead() || !GetWorld()) return;

	SetHealth(Health - Damage);
	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

	if (IsDead())
	{
		Killed(InstigatedBy);
		OnDeath.Broadcast();
	}
	else if (bAutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UTGHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}
}

void UTGHealthComponent::HealUpdate()
{
	SetHealth(Health + HealModifier);
	if(Health == MaxHealth && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void UTGHealthComponent::SetHealth(float NewHealth)
{ 
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
}

void UTGHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UTGHealthComponent, Health);
}

