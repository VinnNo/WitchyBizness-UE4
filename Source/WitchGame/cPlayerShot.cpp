// Fill out your copyright notice in the Description page of Project Settings.

#include "cPlayerShot.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AcPlayerShot::AcPlayerShot()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(11.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AcPlayerShot::OnBeginOverlap);
	CollisionComp->OnComponentHit.AddDynamic(this, &AcPlayerShot::OnHit);

	// Set as root component
	RootComponent = CollisionComp;

	// Try to create the sprite component
	Sprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	if (Sprite)
	{
		Sprite->AlwaysLoadOnClient = true;
		Sprite->AlwaysLoadOnServer = true;
		Sprite->bOwnerNoSee = false;
		Sprite->bAffectDynamicIndirectLighting = true;
		Sprite->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Sprite->SetupAttachment(RootComponent);
		Sprite->SetGenerateOverlapEvents(false);
	}

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 1200.0f;
	ProjectileMovement->MaxSpeed = 1200.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	InitialLifeSpan = 0.0f;

	//Tagging Things
	Tags.Add(TEXT("PlayerShot"));

}

// Called when the game starts or when spawned
void AcPlayerShot::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AcPlayerShot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DelayTimers(DeltaTime);
	FlipGravity();
	if (bIsDebug)
	{
		DebugPrinter();
	}
}

void AcPlayerShot::DelayTimers(float Delta)
{
	if (bLifeTimeDelayActive)
	{
		if (LifeTimeDelayCurrent < LifeTimeDelayMax)
		{
			LifeTimeDelayCurrent += Delta;
		}
		else
		{
			LifeTimeDelayCurrent = 0.0f;
			bLifeTimeDelayActive = false;
			OnDeath();
		}
	}
}

void AcPlayerShot::FlipGravity()
{
	if (FMath::RandRange(1.0f, 100.0f) > ShotAdjustFrequency)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Fired!!!"));
		ShotVelAdjust = GetVelocity();
		if (GetActorRotation() == FRotator(0.0f, 0.0f, 0.0f))
		{
			ShotVelAdjust.X = ProjectileMovement->MaxSpeed;
		}
		else
		{
			float TempSpeed = ProjectileMovement->MaxSpeed;
			ShotVelAdjust.X = -TempSpeed;
		}

		if (ShotVelAdjust.Z == 0.0f)
		{
			if (FMath::RandRange(1, 100) > 50)
			{
				ShotVelAdjust.Z = -ShotAccuracy;
			}
			else
			{
				ShotVelAdjust.Z = ShotAccuracy;
			}
		}
		else if (ShotVelAdjust.Z > 0.0f)
		{
			if (FMath::RandRange(1, 100) > 50)
			{
				ShotVelAdjust.Z = -ShotAccuracy;
			}
			else
			{
				ShotVelAdjust.Z = 0.0f;
			}
		}
		else
		{
			if (FMath::RandRange(1, 100) > 50)
			{
				ShotVelAdjust.Z = 0.0f;
			}
			else
			{
				ShotVelAdjust.Z = ShotAccuracy;
			}
		}
	}
	if (GetVelocity() != ShotVelAdjust)
	{
		ProjectileMovement->Velocity = ShotVelAdjust;
	}
}

void AcPlayerShot::OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (bIsDebug)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Overlapped Began"));
	}
}

void AcPlayerShot::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bIsDebug)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Hit!"));
	}
}
