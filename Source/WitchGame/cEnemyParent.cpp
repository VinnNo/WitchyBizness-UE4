// Fill out your copyright notice in the Description page of Project Settings.

#include "cEnemyParent.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "DrawDebugHelpers.h"

FName AcEnemyParent::SpriteComponentName(TEXT("Sprite0"));

// Sets default values
AcEnemyParent::AcEnemyParent()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Try to create the sprite component
	Sprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(AcEnemyParent::SpriteComponentName);
	if (Sprite)
	{
		Sprite->AlwaysLoadOnClient = true;
		Sprite->AlwaysLoadOnServer = true;
		Sprite->bOwnerNoSee = false;
		Sprite->bAffectDynamicIndirectLighting = true;
		Sprite->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Sprite->SetupAttachment(GetCapsuleComponent());
		static FName CollisionProfileName(TEXT("CharacterMesh"));
		Sprite->SetCollisionProfileName(CollisionProfileName);
		Sprite->SetGenerateOverlapEvents(false);
		Sprite->RelativeLocation = FVector(-SpriteDrawOffset, 0.0f, 0.0f);
	}
	Tags.Add(TEXT("Enemy"));
}

// Called when the game starts or when spawned

void AcEnemyParent::BeginPlay()
{
	Super::BeginPlay();
	STATE = IDLE;
}

// Called every frame
void AcEnemyParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AcEnemyParent::GetHit(float InDamage)
{
	if (STATE != HIT)
	{
		if (InDamage > 0.0f)
		{
			STATE = HIT;
			bNewState = true;
			HpCurrent -= InDamage;
			if (HpCurrent < 0.0f)
			{
				HpCurrent = 0.0f;
			}
		}
	}
}
// Called to bind functionality to input
/*
void AcEnemyParent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
*/

