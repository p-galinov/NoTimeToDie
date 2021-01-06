// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"
#include "Engine/World.h"


void AProjectileWeapon::Fire()
{
	AActor* MyOwner = GetOwner();

	if (MyOwner && MyProjectileClass)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


		GetWorld()->SpawnActor<AActor>(MyProjectileClass, MuzzleLocation, EyeRotation, SpawnParams);
	}

}