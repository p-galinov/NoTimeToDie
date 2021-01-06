// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "Boost.h"

// Sets default values
APickUp::APickUp()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(75.0f);
	RootComponent = SphereComp;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->SetRelativeRotation(FRotator(90, 0.0f, 0.0f));
	DecalComp->DecalSize = FVector(64, 75, 75);
	DecalComp->SetupAttachment(RootComponent);

	CoolDownDuration = 10.0f;
}

// Called when the game starts or when spawned
void APickUp::BeginPlay()
{
	Super::BeginPlay();
	
	Respawn();
}

void APickUp::Respawn()
{
	if (BoostClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PowerUpClass is nullptr in %s please update blueprint"), *GetName());
		return; 
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	BoostInstance= GetWorld()->SpawnActor<ABoost>(BoostClass, GetTransform(), SpawnParams);
}

void APickUp::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (BoostInstance)
	{
		BoostInstance->ActivateBoost();
		BoostInstance = nullptr;

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &APickUp::Respawn, CoolDownDuration);
	}
}



