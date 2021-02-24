// Fill out your copyright notice in the Description page of Project Settings.

#include "cPickupParent.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "cWitch.h"

// Sets default values
AcPickupParent::AcPickupParent()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(11.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	//CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AcPickupParent::OnBeginOverlap);
	//CollisionComp->OnComponentHit.AddDynamic(this, &AcPickupParent::OnHit);

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
	ProjectileMovement->InitialSpeed = 500.0f;
	ProjectileMovement->MaxSpeed = PickupSpeed1;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bIsHomingProjectile = false;

	InitialLifeSpan = 0.0f;

	//Tagging Things
	Tags.Add(TEXT("Pickup"));

}

// Called when the game starts or when spawned
void AcPickupParent::BeginPlay()
{
	Super::BeginPlay();

	TargetActor = Cast<AcWitch>(TargetActor);
}

void AcPickupParent::StartSeekingEvent()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void AcPickupParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DoSeeking(DeltaTime);
	DelayTimers(DeltaTime);
}

void AcPickupParent::DoSeeking(float Delta)
{
	if (!bHasTriggeredSeeking)
	{
		FVector TemporaryTargetLocation = FVector(0.0f, 0.0f, 0.0f);
		TargetActor = Cast<AcWitch>(TargetActor);
		if (TargetActor != nullptr)
		{
			TemporaryTargetLocation = TargetActor->GetActorLocation();
			GetMove()->HomingTargetComponent = TargetActor->GetRootComponent();
			GetMove()->bIsHomingProjectile = true;
			GetMove()->Velocity += (TargetActor->GetActorLocation() * Delta);
			bHasTriggeredSeeking = true;
			bSpeedPickupDelayActive = true;
		}
	}
}

void AcPickupParent::DelayTimers(float Delta)
{
	if (bSpeedPickupDelayActive)
	{
		if (SpeedPickupDelayCurrent < SpeedPickupDelayMax)
		{
			SpeedPickupDelayCurrent += Delta;
		}
		else
		{
			GetMove()->MaxSpeed = PickupSpeed2;
			SpeedPickupDelayCurrent = 0.0f;
			bSpeedPickupDelayActive = false;
		}
	}
}



