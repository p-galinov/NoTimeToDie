// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "MyGameMode.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	TeamNum = 255;
	DefaultHealth = 100;
	bIsDead = false;

}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
	}

	Health = DefaultHealth;
}

void UHealthComponent::HandleTakeAnyDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (Damage <= 0.0f || bIsDead)
	{
		return;
	}

	if (DamageCauser != DamagedActor && IsFriendly(GetOwner(), DamageCauser))
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	UE_LOG(LogTemp, Log, TEXT("Health changed: %s"), *FString::SanitizeFloat(Health));

	bIsDead = Health <= 0.0f;

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);

	if (bIsDead)
	{
		AMyGameMode* GM = Cast<AMyGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);
		}
	}

}

float UHealthComponent::GetHealth() const
{
	return Health;
}

void UHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.0f || Health <=0.0f)
	{
		return;
	}

	Health = FMath::Clamp(Health + HealAmount, 0.0f, DefaultHealth);

	UE_LOG(LogTemp, Log, TEXT("Health changed: %s (+%s)"), *FString::SanitizeFloat(Health), *FString::SanitizeFloat(HealAmount));

	OnHealthChanged.Broadcast(this, Health, -HealAmount, nullptr, nullptr, nullptr);

}

bool UHealthComponent::IsFriendly(AActor * ActorA, AActor * ActorB)
{
	if (ActorA == nullptr || ActorB == nullptr)
	{
		return false;
	}

	UHealthComponent* HealthCompA =Cast<UHealthComponent>(ActorA->GetComponentByClass(UHealthComponent::StaticClass()));
	UHealthComponent* HealthCompB = Cast<UHealthComponent>(ActorB->GetComponentByClass(UHealthComponent::StaticClass()));


	if (HealthCompA == nullptr || HealthCompB == nullptr)
	{
		//Assume friendly
		return true;
	}

	return HealthCompA->TeamNum == HealthCompB->TeamNum;
}


