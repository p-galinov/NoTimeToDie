// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Weapon.h"
#include "Shooter.h"
#include "Components/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "..\Public\MainCharacter.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	ZoomedFOV = 65.0f;
	ZoomInterSpeed = 20.0f;

	WeaponAttachSocketName = "WeaponSocket";
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	DefaultFOV = CameraComp->FieldOfView;
	HealthComp->OnHealthChanged.AddDynamic(this, &AMainCharacter::OnHealthChanged);

		//Spawn a default weapon
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (CurrentWeapon)
		{
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
		}
	
}

void AMainCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void AMainCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);

}

void AMainCharacter::BeginCrouch()
{
	Crouch();
}

void AMainCharacter::EndCrouch()
{
	UnCrouch();
}

void AMainCharacter::BeginZoom()
{
	bWantsToZoom = true;
}

void AMainCharacter::EndZoom()
{
	bWantsToZoom = false;
}

void AMainCharacter::StartFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}

}

void AMainCharacter::EndFire()
{

	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void AMainCharacter::LaunchGranade()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->LaunchGranade();
	}
}

void AMainCharacter::OnHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{

	if (Health <= 0.0f && !bDied)
	{
		EndFire();
		bDied = true;
		//Die
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DetachFromControllerPendingDestroy();
		SetLifeSpan(10.0f);
		
	}

}


// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;

	float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterSpeed);

	CameraComp->SetFieldOfView(NewFOV);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed,this, &AMainCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released,this, &AMainCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::Jump);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AMainCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AMainCharacter::EndZoom);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMainCharacter::EndFire);
	PlayerInputComponent->BindAction("LaunchGranade", IE_Pressed, this, &AMainCharacter::LaunchGranade);

}

FVector AMainCharacter::GetPawnViewLocation() const
{
	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}
