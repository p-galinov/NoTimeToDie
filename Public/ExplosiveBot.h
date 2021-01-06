// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ExplosiveBot.generated.h"

class UHealthComponent;
class USphereComponent;
class USoundCue;

UCLASS()
class SHOOTER_API AExplosiveBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AExplosiveBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UHealthComponent* HealthComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USphereComponent* SphereComp;

	UFUNCTION()
	void HandleTakeDamage(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	FVector GetNextPathPoint();

	//Next point in navigation
	FVector NextPathPoint;

	UPROPERTY(EditDefaultsOnly, Category = "ExplosiveBot")
	float MovementForce;

	UPROPERTY(EditDefaultsOnly, Category = "ExplosiveBot")
	bool bUseVelocityChange;

	UPROPERTY(EditDefaultsOnly, Category = "ExplosiveBot")
	float RequiredDistanceToTarget;

	//Dynamic material to pulse on damage
	UMaterialInstanceDynamic* MatInst;

	void SelfDestruct();

	UPROPERTY(EditDefaultsOnly, Category = "ExplosiveBot")
	UParticleSystem* ExplosionEffect;

	bool bExploded;

	bool bStartedSelfDestruction;

	UPROPERTY(EditDefaultsOnly, Category = "ExplosiveBot")
	float ExplosionRadius;

	UPROPERTY(EditDefaultsOnly, Category = "ExplosiveBot")
	float ExplosionDamage;

	UPROPERTY(EditDefaultsOnly, Category = "ExplosiveBot")
	float SelfDamageInterval;

	FTimerHandle TimerHandle_SelfDamage;

	void DamageSelf();

	UPROPERTY(EditDefaultsOnly, Category = "ExplosiveBot")
	USoundCue* SelfDestructSound;

	UPROPERTY(EditDefaultsOnly, Category = "ExplosiveBot")
	USoundCue* ExplodeSound;

	FTimerHandle Timerhandle_RefreshPath;

	void RefreshPath();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
