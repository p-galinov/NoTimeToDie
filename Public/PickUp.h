// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUp.generated.h"

class USphereComponent;
class UDecalComponent;
class ABoost;

UCLASS()
class SHOOTER_API APickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDecalComponent* DecalComp;

	UPROPERTY(EditInstanceOnly, Category = "PickUp")
	TSubclassOf<ABoost> BoostClass;

	void Respawn();

	ABoost* BoostInstance;

	UPROPERTY(EditInstanceOnly, Category = "PickUp")
	float CoolDownDuration;

	FTimerHandle TimerHandle_RespawnTimer;

public:	

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
