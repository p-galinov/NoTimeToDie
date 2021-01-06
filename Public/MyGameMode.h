// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameMode.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*, KillerController);

UCLASS()
class SHOOTER_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	//BP to spawn single bot
	UFUNCTION(BlueprintImplementableEvent, Category = "Gamemode")
	void SpawnNewBot();

	//Start Spawning bots
	void StartWave();

	//End Spawning bots
	void EndWave();

	//Set timer for next wave
	void PrepareForNextWave();

	FTimerHandle TimerHandle_BotSpawner;

	FTimerHandle TimerHandle_NextWaveStart;

	//Bots to spawn in current wave
	int32 NumOfBotsToSpawn;

	int32 WaveCount;

	void SpawnBotTimerElapsed();

	UPROPERTY(EditDefaultsOnly, Category="Gamemode")
	float TimeBetweenWaves;

	void CheckWaveState();

	void CheckAnyPlayerAlive();

	void GameOver();

	void RestartDeadPlayers();

public:

	AMyGameMode();

	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(BlueprintAssignable, Category="Gamemode")
	FOnActorKilled OnActorKilled;

};
