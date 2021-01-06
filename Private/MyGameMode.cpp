// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/HealthComponent.h"
#include "MyPlayerState.h"


AMyGameMode::AMyGameMode()
{
	TimeBetweenWaves = 5.0f;

	PlayerStateClass = AMyPlayerState::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
	

}

void AMyGameMode::CheckWaveState()
{
	bool isPreparingForNextWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);

	if (NumOfBotsToSpawn > 0 || isPreparingForNextWave)
	{
		return;
	}

	bool bIsAnyBotAlive = false;

	//list of pawns in the world
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* TestPawn = It->Get();

		// looking for player pawn to exclude it 
		if (TestPawn == nullptr || TestPawn->IsPlayerControlled())
		{
			continue;
		}

		UHealthComponent* HealthComp = Cast<UHealthComponent>(TestPawn->GetComponentByClass(UHealthComponent::StaticClass()));
		if (HealthComp && HealthComp->GetHealth() > 0.0f)
		{
			bIsAnyBotAlive = true;
			break;
		}
	}

	if (!bIsAnyBotAlive)
	{
		PrepareForNextWave();
	}

}

void AMyGameMode::CheckAnyPlayerAlive()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = Cast<APlayerController>(It->Get());
		if (PC && PC->GetPawn())
		{
			APawn* MyPawn = PC->GetPawn();
			UHealthComponent* HealthComp = Cast<UHealthComponent>(MyPawn->GetComponentByClass(UHealthComponent::StaticClass()));
			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f)
			{
				//Player is still alive
				return;
			}
		}
		
	}

	//No Player alive
	GameOver();
}

void AMyGameMode::GameOver()
{
	EndWave();



	UE_LOG(LogTemp, Log, TEXT("Game over"));
}

void AMyGameMode::StartWave()
{
	WaveCount++;

	NumOfBotsToSpawn = 2 * WaveCount;

	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &AMyGameMode::SpawnBotTimerElapsed, 1.0f, true, 0.0f);


}

void AMyGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);

	
}

void AMyGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &AMyGameMode::StartWave, TimeBetweenWaves, false);

	RestartDeadPlayers();

}

void AMyGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();

	NumOfBotsToSpawn--;

	if (NumOfBotsToSpawn <= 0)
	{
		EndWave();
	}
}

void AMyGameMode::StartPlay()
{
	Super::StartPlay();

	PrepareForNextWave();
}

void AMyGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckWaveState();

	CheckAnyPlayerAlive();
}

void AMyGameMode::RestartDeadPlayers()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = Cast<APlayerController>(It->Get());
		if (PC && PC->GetPawn() == nullptr)
		{
			RestartPlayer(PC);
		}
	}
}