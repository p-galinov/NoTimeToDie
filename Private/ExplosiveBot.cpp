// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBot.h"
#include "Kismet/GameplayStatics.h"
#include "AI/NavigationSystemBase.h"
#include "Public/NavigationSystem.h"
#include "GameFramework/Character.h"
#include "Components/HealthComponent.h"
#include "MainCharacter.h"
#include"NavigationPath.h"
#include"Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
AExplosiveBot::AExplosiveBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	HealthComp=CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &AExplosiveBot::HandleTakeDamage);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetSphereRadius(200);
	SphereComp->SetupAttachment(RootComponent);

	bUseVelocityChange = false;
	MovementForce = 1000;
	RequiredDistanceToTarget = 1000;

	ExplosionDamage = 40;
	ExplosionRadius = 350;

	SelfDamageInterval = 0.5f;
}

// Called when the game starts or when spawned
void AExplosiveBot::BeginPlay()
{
	Super::BeginPlay();
	
	 NextPathPoint = GetNextPathPoint();

}

void AExplosiveBot::HandleTakeDamage(UHealthComponent * OwningHealthComp, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{

	if(MatInst == nullptr)
	{ 
	MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
	}


	if (MatInst) 
	{
		MatInst->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);

	}
	UE_LOG(LogTemp, Log, TEXT("Health %s of %s"), *FString::SanitizeFloat(Health), *GetName());

	//Explode hipoints=0
	if (Health <= 0.0f)
	{
		SelfDestruct();
	}
}

FVector AExplosiveBot::GetNextPathPoint()
{
	//AActor* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	AActor* BestTarget = nullptr;
	float NearesTargettDistance = FLT_MAX;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* TestPawn = It->Get();

		if (TestPawn == nullptr || UHealthComponent::IsFriendly(TestPawn, this))
		{
			continue;
		}

		UHealthComponent* TestPawnHealthComp = Cast<UHealthComponent>(TestPawn->GetComponentByClass(UHealthComponent::StaticClass()));
		if (HealthComp && TestPawnHealthComp->GetHealth() > 0.0f)
		{
			float Distance = (TestPawn->GetActorLocation() - GetActorLocation()).Size();
			if (Distance<NearesTargettDistance)
			{
				BestTarget = TestPawn;
				NearesTargettDistance = Distance;
			}
			
		}
	}

	if (BestTarget)
	{
		UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), BestTarget);


		GetWorldTimerManager().ClearTimer(Timerhandle_RefreshPath);
		GetWorldTimerManager().SetTimer(Timerhandle_RefreshPath, this, &AExplosiveBot::RefreshPath, 5.0f, false);

		if (NavPath && NavPath->PathPoints.Num() > 1)
		{
			//Return next point in the path
			return NavPath->PathPoints[1];
		}
	}

	return GetActorLocation();
}

void AExplosiveBot::SelfDestruct()
{
	if (bExploded)
	{
		return;
	}

	bExploded = true;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	//Apply damage
	UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);

	UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation());

	Destroy();
}

// Called every frame
void AExplosiveBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();

	if (DistanceToTarget <= RequiredDistanceToTarget)
	{
		NextPathPoint = GetNextPathPoint();
	}
	else
	{
		//Move to next target
		FVector ForceDirection = NextPathPoint - GetActorLocation();
		ForceDirection.GetSafeNormal();
		ForceDirection.Normalize();
		ForceDirection *= MovementForce;

		MeshComp->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
	}

}

void AExplosiveBot::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if(!bStartedSelfDestruction)
	{
		AMainCharacter* PlayerPawn = Cast<AMainCharacter>(OtherActor);
		if (PlayerPawn && !UHealthComponent::IsFriendly(OtherActor, this))
		{
			//Start self destruction squence
			GetWorldTimerManager().SetTimer(TimerHandle_SelfDamage, this, &AExplosiveBot::DamageSelf, SelfDamageInterval, true, 0.0f);

			bStartedSelfDestruction = true;

			UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent);
		}
	}
}

void AExplosiveBot::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, 20, GetInstigatorController(), this, nullptr);
}

void AExplosiveBot::RefreshPath()
{
	NextPathPoint = GetNextPathPoint();
}

