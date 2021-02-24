// Fill out your copyright notice in the Description page of Project Settings.

#include "cDemonPlasma.h"

// Fill out your copyright notice in the Description page of Project Settings.

#include "cEnemyWalkerBase.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"

AcDemonPlasma::AcDemonPlasma()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AcDemonPlasma::OnBeginOverlap);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AcDemonPlasma::OnHit);
	GetCapsuleComponent()->SetCapsuleHalfHeight(32.0f);
	GetCapsuleComponent()->SetCapsuleRadius(25.0f);

	GetSprite()->RelativeLocation = FVector(-3.5f, 0.0f, 0.0f);

	//The base component that everything rotates on
	RotationalComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RotationalComponent"));
	RotationalComponent->SetupAttachment(RootComponent);

	TracerComp = CreateDefaultSubobject<USceneComponent>(TEXT("TracerComp"));
	TracerComp->SetupAttachment(RotationalComponent);
	TracerComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	TracerComp->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);

	EyesSprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("EyesSprite"));
	EyesSprite->AlwaysLoadOnClient = true;
	EyesSprite->AlwaysLoadOnServer = true;
	EyesSprite->bOwnerNoSee = false;
	EyesSprite->bAffectDynamicIndirectLighting = true;
	EyesSprite->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	EyesSprite->SetupAttachment(GetSprite());
	EyesSprite->SetGenerateOverlapEvents(false);
	EyesSprite->RelativeLocation = FVector(0.0f, 0.1f, 0.0f);


	//***********************( Setup for Character Movement )*************************
	GetCharacterMovement()->GravityScale = 1.75f;
	GetCharacterMovement()->MaxAcceleration = 512.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.0f;
	GetCharacterMovement()->Mass = 80.0f;
	GetCharacterMovement()->GroundFriction = 9.5f;
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->AirControlBoostMultiplier = 1.25f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));
	//Make flat floors since I don't use slopes
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
	//----------------------------------------------------------------------------------
}

// Called when the game starts or when spawned
void AcDemonPlasma::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AcDemonPlasma::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateStates();
	UpdateAnimation();
	DelayTimers(DeltaTime);
	if (HpCurrent == 0.0f)
	{
		if (STATE != TRA1)
		{
			STATE = TRA1;
			contactDamageCurrent = 0.0f;
			bNewState = true;
		}
	}
	if (bShouldDoTurn)
	{
		DoTurnAround();
		bShouldDoTurn = false;
	}
}

void AcDemonPlasma::OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	//Stuff
}

void AcDemonPlasma::OnHit(UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (STATE == RUN)
	{
		//ChangeDirection();
	}
}


void AcDemonPlasma::MoveH()
{
	if (STATE == RUN)
	{
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Facing);
	}
}

void AcDemonPlasma::ChangeDirection()
{
	FVector SpriteScale = Sprite->GetComponentScale();

	if (Facing == 1)
	{
		Facing = -1;
		SpriteScale.X = -1.0f;
		Sprite->SetRelativeScale3D(SpriteScale);
		RotCompDir = FRotator(0.0f, 180.0f, 0.0f);
		GetSprite()->RelativeLocation = FVector(SpriteXOffset, 0.0f, 0.0f);
	}
	else
	{
		Facing = 1;
		SpriteScale.X = 1.0f;
		Sprite->SetRelativeScale3D(SpriteScale);
		RotCompDir = FRotator(0.0f, 0.0f, 0.0f);
		GetSprite()->RelativeLocation = FVector(-SpriteXOffset, 0.0f, 0.0f);
	}
}

void AcDemonPlasma::RunFloorTrace()
{
	FHitResult OutHit;
	FVector TempStart = GetTracerComp()->GetComponentLocation();
	TempStart.X = TempStart.X + TraceDistance;
	FVector Start = TempStart;// GetTracerComp()->GetComponentLocation();
	FVector DownVector = Start;
	DownVector.Z = DownVector.Z - ((GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) + FloorDistance);
	FVector End = DownVector;
	FCollisionQueryParams CollisionParams;
	if (bIsDebug)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.05f);
	}

	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

	if (!isHit)
	{
		if (!OutHit.bBlockingHit)
		{
			ChangeDirection();
		}
	}
}

void AcDemonPlasma::RunWallTrace()
{
	FHitResult OutHit;
	FVector Start = GetRotationalComponent()->GetComponentLocation();
	FVector ForwardVector = GetRotationalComponent()->GetForwardVector();
	float TraceMod = GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	FVector End = ((ForwardVector * (TraceMod + TraceDistance)) + Start);
	FCollisionQueryParams CollisionParams;
	if (bIsDebug)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.05f);
	}

	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

	if (isHit)
	{
		if (OutHit.bBlockingHit)
		{
			if (bIsDebug)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Hit: %s!"), *OutHit.GetActor()->GetName()));
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Hit: %s!"), *OutHit.ImpactPoint.ToString()));
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Hit: %s!"), *OutHit.ImpactNormal.ToString()));
			}
			DoTurnAround();
		}
	}
}

void AcDemonPlasma::DoTurnAround()
{
	ChangeDirection();
	bTurnDelayActive = true;
	TurnDelayCurrent = 0.0f;
	bCanTurn = false;
}

void AcDemonPlasma::UpdateStates()
{
	if (bNewState)
	{
		RunNewStates();
		return;
	}
	switch (STATE)
	{
	case 10:
	{
		//Stuff
		break;
	}

	case 11:
	{
		MoveH();
		if (bCanTurn)
		{
			RunWallTrace();
			RunFloorTrace();
		}
		break;
	}
	case 14:
	{
		if (!bShouldDie)
		{
			bShouldDie = true;
			DeathSequenceEvent();
		}
		break;
	}
	}
}

void AcDemonPlasma::RunNewStates()
{
	if (bNewState)
	{
		switch (STATE)
		{
			//IDLE
		case 10:
		{
			if (!bIdleDelayActive)
			{
				bIdleDelayActive = true;
				IdleDelayCurrent = 0.0f;
			}
			DesiredAnimation = IdleAnim;
			DesiredEyesAnimation = IdleEyesAnim;
			GetCharacterMovement()->Velocity = FVector(0.0f, 0.0f, 0.0f);
			break;
		}
		//RUN
		case 11:
		{
			DesiredAnimation = RunAnim;
			DesiredEyesAnimation = RunEyesAnim;
			break;
		}
		//HIT
		case 13:
		{
			if (!bHitDelayActive)
			{
				bHitDelayActive = true;
				HitDelayCurrent = 0.0f;
				contactDamageCurrent = 0.0f;
			}
			DesiredAnimation = HitAnim;
			DesiredEyesAnimation = HitEyesAnim;
			GetCharacterMovement()->Velocity = FVector(0.0f, 0.0f, 0.0f);
			break;
		}
		case 14:
		{
			DesiredAnimation = Trans1Anim;
			GetSprite()->SetLooping(false);
			break;
		}
		}
		bNewState = false;
	}
}

void AcDemonPlasma::DelayTimers(float Delta)
{
	if (bIdleDelayActive)
	{
		if (IdleDelayCurrent < IdleDelayMax)
		{
			IdleDelayCurrent += Delta;
		}
		else
		{
			IdleDelayCurrent = 0.0f;
			bIdleDelayActive = false;
			STATE = RUN;
			bNewState = true;
		}
	}

	if (bHitDelayActive)
	{
		if (HitDelayCurrent < HitDelayMax)
		{
			HitDelayCurrent += Delta;
		}
		else
		{
			HitDelayCurrent = 0.0f;
			bHitDelayActive = false;
			contactDamageCurrent = contactDamageMax;
			STATE = RUN;
			bNewState = true;
		}
	}

	if (bTurnDelayActive)
	{
		//if (STATE != RUN)
		//{
			TurnDelayCurrent = 0.0f;
			bTurnDelayActive = false;
			bCanTurn = true;
			//return;
		//}

		if (TurnDelayCurrent < TurnDelayMax)
		{
			TurnDelayCurrent += Delta;
		}
		else
		{
			TurnDelayCurrent = 0.0f;
			bTurnDelayActive = false;
			bCanTurn = true;
		}
	}
}

void AcDemonPlasma::UpdateAnimation()
{
	FVector SpriteLoc = GetTransform().InverseTransformPosition(Sprite->GetComponentLocation());
	FVector SpriteLocOffset = SpriteLoc;

	if (Facing == 1)
	{
		SpriteLocOffset.X = -SpriteDrawOffset;
		Sprite->SetRelativeLocation(SpriteLocOffset);
		RotCompDir = FRotator(0.0f, 0.0f, 0.0f);
	}
	else
	{
		SpriteLocOffset.X = SpriteDrawOffset;
		Sprite->SetRelativeLocation(SpriteLocOffset);
		RotCompDir = FRotator(0.0f, -180.0f, 0.0f);
	}

	if (RotationalComponent->RelativeRotation != RotCompDir)
	{
		RotationalComponent->RelativeRotation = RotCompDir;
	}

	if (GetSprite()->GetFlipbook() != DesiredAnimation)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}

	if (GetEyesSprite()->GetFlipbook() != DesiredEyesAnimation)
	{
		GetEyesSprite()->SetFlipbook(DesiredEyesAnimation);
	}
}

//void AcDemonPlasma::DeathSequenceEvent()
//{
	//Stuff
//}
