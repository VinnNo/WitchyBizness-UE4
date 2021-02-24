// Fill out your copyright notice in the Description page of Project Settings.

#include "cBounceBlockParent.h"
#include "cWitch.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AcBounceBlockParent::AcBounceBlockParent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	CollisionComp->SetBoxExtent(FVector(18.0f, 50.0f, 10.0f));
	CollisionComp->BodyInstance.SetCollisionProfileName("Interactive");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AcBounceBlockParent::OnBeginOverlap);

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

	//Tagging Things
	Tags.Add(TEXT("Interactive"));
}

// Called when the game starts or when spawned
void AcBounceBlockParent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AcBounceBlockParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AcBounceBlockParent::OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (TargetActor != nullptr)
	{
		if (OtherActor == TargetActor)
		{
			FVector TempVel = TargetActor->GetVelocity();
			if (TempVel.Z < 0.0f)
			{
				DoOverlap();
			}
		}
	}
}

