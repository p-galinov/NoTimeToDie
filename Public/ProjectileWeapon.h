// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()
	
protected:

	virtual void Fire() override;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectileWeapon")
	TSubclassOf<AActor> MyProjectileClass;
};
