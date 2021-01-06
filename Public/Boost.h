// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boost.generated.h"

UCLASS()
class SHOOTER_API ABoost : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoost();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Time between boost ticks
	UPROPERTY(EditDefaultsOnly, Category = "Boosts")
	float BoostInterval;

	//Total times we apply the boost effect
	UPROPERTY(EditDefaultsOnly, Category = "Boosts")
	int32 NumberOfTicks;

	FTimerHandle TimeHandle_BoostTick;

	//Total number of ticks
	int32 TicksPassed;

	UFUNCTION()
	void OnTickBoost();

public:	

	void ActivateBoost();

	UFUNCTION(BlueprintImplementableEvent, Category = "Boosts")
	void OnActivate();

	UFUNCTION(BlueprintImplementableEvent, Category = "Boosts")
	void OnBoostTicked();

	UFUNCTION(BlueprintImplementableEvent, Category = "Boosts")
	void OnExpired();

};
