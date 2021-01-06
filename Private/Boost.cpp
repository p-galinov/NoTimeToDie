// Fill out your copyright notice in the Description page of Project Settings.


#include "Boost.h"

// Sets default values
ABoost::ABoost()
{
	BoostInterval = 0.0f;
	NumberOfTicks = 0;

}

// Called when the game starts or when spawned
void ABoost::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABoost::OnTickBoost()
{
	TicksPassed++;

	OnBoostTicked();

	if (TicksPassed >= NumberOfTicks)
	{
		OnExpired();

		// Delete Timer
		GetWorldTimerManager().ClearTimer(TimeHandle_BoostTick);
	}
}

void ABoost::ActivateBoost()
{
	OnActivate();

	if (BoostInterval > 0)
	{
		GetWorldTimerManager().SetTimer(TimeHandle_BoostTick, this, &ABoost::OnTickBoost, BoostInterval, true);

	}
	else
	{
		OnTickBoost();
	}
}




