// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Barrel.generated.h"


class UHealthComponent;
class UStaticMeshComponent;
class URadialForceComponent;
class UParticleSystem;


UCLASS()
class SHOOTER_API ABarrel : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABarrel();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UHealthComponent* HealthComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		URadialForceComponent* RadialForceComp;

	/** Sound to play when explodes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class USoundBase* ExplodeSound;

	UFUNCTION()
		void OnHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType,
			class AController* InstigatedBy, AActor* DamageCauser);

	
	bool bExploded;

	//UFUNCTION()
	//	void OnRep_Exploded();

	/* Impulse applied to the barrel mesh when it explodes to boost it up a little */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
		float ExplosionImpulse;

	/* Particle to play when health reached zero */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
		UParticleSystem* ExplosionEffect;

	/* The material to replace the original on the mesh once exploded (a blackened version) */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
		UMaterialInterface* ExplodedMaterial;

};