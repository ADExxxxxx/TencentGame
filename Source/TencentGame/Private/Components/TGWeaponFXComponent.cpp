
#include "Components/TGWeaponFXComponent.h"
#include "Components/DecalComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

UTGWeaponFXComponent::UTGWeaponFXComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UTGWeaponFXComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void UTGWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{
	auto ImpactData = DefaultImpactData;

	if (Hit.PhysMaterial.IsValid())
	{
		const auto PhysMat = Hit.PhysMaterial.Get();
		if (ImpactDataMap.Contains(PhysMat))
		{
			ImpactData = ImpactDataMap[PhysMat];
		}
	}

	// niagara
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), 
		ImpactData.NiagaraEffect,                              
		Hit.ImpactPoint,                                       
		Hit.ImpactNormal.Rotation());

	// decal
	auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),  
		ImpactData.DecalData.Material,                                        
		ImpactData.DecalData.Size,                                            
		Hit.ImpactPoint,                                                      
		Hit.ImpactNormal.Rotation());
	if (DecalComponent)
	{
		
		DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
	}

	// sound
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactData.Sound, Hit.ImpactPoint);
}




