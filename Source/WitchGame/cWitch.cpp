// Fill out your copyright notice in the Description page of Project Settings.

#include "cWitch.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"

FName AcWitch::SpriteComponentName(TEXT("Sprite0"));

AcWitch::AcWitch(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.DoNotCreateDefaultSubobject(ACharacter::MeshComponentName))
{
	// Try to create the sprite component
	Sprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(AcWitch::SpriteComponentName);
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
		Sprite->SetFlipbook(IdleAnim);
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	SpringArm->TargetArmLength = 900.0f;
	SpringArm->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bAbsoluteRotation = true;

	CamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
	CamComp->ProjectionMode = ECameraProjectionMode::Perspective;
	CamComp->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	CamComp->bAutoActivate = true;
	CamComp->bUsePawnControlRotation = false;

	RotComp = CreateDefaultSubobject<USceneComponent>(TEXT("RotComp"));
	RotComp->SetupAttachment(RootComponent);

	ShotPosComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ShotPosComp"));
	ShotPosComp->SetupAttachment(RotComp);

	GetCapsuleComponent()->SetCapsuleHalfHeight(35.0f);
	GetCapsuleComponent()->SetCapsuleRadius(17.0f);

	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
	GetCharacterMovement()->GravityScale = 1.75f;
	GetCharacterMovement()->MaxAcceleration = 512.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.0f;
	GetCharacterMovement()->Mass = 80.0f;
	GetCharacterMovement()->GroundFriction = 9.5f;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->AirControlBoostMultiplier = 1.25f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

}

// Called to bind functionality to input
void AcWitch::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("kJump", IE_Pressed, this, &AcWitch::DoJumped);
	PlayerInputComponent->BindAction("kJump", IE_Released, this, &AcWitch::DoStopJump);
	PlayerInputComponent->BindAxis("kHor", this, &AcWitch::DoSetMovement);//HorizontalMovement);
	PlayerInputComponent->BindAction("kShootAction", IE_Pressed, this, &AcWitch::DoShotPressed);
	PlayerInputComponent->BindAction("kShootAction", IE_Released, this, &AcWitch::DoShotReleased);
}

// Called when the game starts or when spawned
void AcWitch::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AcWitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateStates();
	UpdateAnimations();
	TimerDelays(DeltaTime);

	if (kShotHeld || kShotReleased)
	{
		DoCharge(DeltaTime);
	}
	FVector Vel = GetVelocity();
	if (bIsOnWall && Vel.Z < MaxFallSpeed / 4)
	{
		GetCharacterMovement()->Velocity.Z = MaxFallSpeed / 4;
	}
	if (Vel.Z < MaxFallSpeed)
	{
		GetCharacterMovement()->Velocity.Z = MaxFallSpeed;
	}
	StoredDelta = DeltaTime;
	OnWall();
}

void AcWitch::HorizontalMovement(float Value)
{
	//Bail out
	if (Value == 0.0f)
	{
		return;
	}

	//For sanitizing input.
	bool kLeft = false;
	bool kRight = false;

	//See if we passed a threshold
	if (Value > InputThreshold)
	{
		kRight = true;
	}

	if (Value < -InputThreshold)
	{
		kLeft = true;
	}

	//Set the desired facing direction, to hold input value and sprite Scale.X
	if ((kRight && !kLeft) || (!kRight && kLeft))
	{
		if (kRight)
		{
			Facing = 1;
		}

		if (kLeft)
		{
			Facing = -1;
		}
		//Apply direction value to movement, using facing.
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Facing);
	}
}

void AcWitch::DoSetMovement(float Value)
{
	//Bail out
	if (Value == 0.0f)
	{
		return;
	}
	float tempVal = 0.0f;

	//See if we passed a threshold
	if (Value > InputThreshold)
	{
		tempVal = 1.0f;
	}

	if (Value < -InputThreshold)
	{
		tempVal = -1.0f;
	}
	if (!isTouchScreen)
	{
		HorizontalMovement(tempVal);
	}
}

void AcWitch::DoJumped()
{
	if (OnGround())
	{
		//Do normal jumps while on the ground.
		Jump();
	}
	else if (bIsOnWall)
	{
		FVector tempLaunch = GetVelocity();
		tempLaunch.Z = LaunchHeight;
		tempLaunch.X = WallPushOff * -Facing;
		LaunchCharacter(tempLaunch, true, true);
	}
	else if (bHasSkill_DoubleJump)
	{
		if (LaunchCount > 0)
		{
			//Use launch to customize the double jump.
			FVector tempLaunch = GetVelocity();
			tempLaunch.Z = LaunchHeight;
			LaunchCharacter(tempLaunch, true, true);
			LaunchCount -= 1;
			OnDoubleJumped();
		}
	}
}

void AcWitch::Landed(const FHitResult& Hit)
{
	LaunchCount = LaunchCountMax;
}

void AcWitch::DoStopJump()
{
	FVector vel = GetVelocity();
	if (vel.Z > 0.0f)
	{
		vel.Z *= 0.25f;
		GetCharacterMovement()->Velocity = vel;
	}
}

void AcWitch::DoShotPressed()
{
	kShotHeld = true;
	if (ManaCurrent > ShotCost)
	{
		if (ShotCount > 0)
		{
			bAttackAnimation = true;
			bAttackAnimationDelayActive = true;
			//Let the blueprint know if we passed shooting prereqs.
			MakeShotEvent();
			ShotCount -= 1;
			ManaCurrent -= 1;
			StoredAnimationTime = Sprite->GetPlaybackPosition();
			/*
			if (!IsCharging && IsCharged)
			{
				IsCharging = true;
			}
			*/
			if (!bFireRateDelayActive)
			{
				bFireRateDelayActive = true;
				FireRateDelayCurrent = 0.0f;
			}
		}
	}
}

void AcWitch::DoShotReleased()
{
	kShotReleased = true;
	if (kShotHeld)
	{
		kShotHeld = false;
	}
	if (CurrentSpriteColor != Color_White)
	{
		CurrentSpriteColor = Color_White;
		Sprite->SetSpriteColor(CurrentSpriteColor);
	}
	ShotReleasedEvent();
}

void AcWitch::DoCharge(float Delta)
{
	//Bailout
	if (kShotReleased)
	{
		//IsCharged
		if (ChargeState > 0)
		{
			MakeShotEvent();
			IsCharged = false;
			bAttackAnimation = true;
			bAttackAnimationDelayActive = true;
			StoredAnimationTime = Sprite->GetPlaybackPosition();
			CurrentSpriteColor = Color_White;
			Sprite->SetSpriteColor(CurrentSpriteColor);
			ChargeState = 0;
		}
		/*
		if (IsCharging)
		{
			IsCharging = false;
			ChargeState = 0;
		}
		*/
		kShotReleased = false;
		ChargeTimeCurrent = 0.0f;
		return;
	}
	if (!kShotHeld)
	{
		return;
	}

	if (kShotHeld && (!IsCharging && !IsCharged))
	{
		if ((ManaCurrent - 1.0f) >= 0.0f)
		{
			IsChargingEvent(); //Call out to blueprints
			IsCharging = true;
			if (ChargeTimeCurrent != 0.0f)
			{
				ChargeTimeCurrent = 0.0f;
			}
		}
	}

	if (IsCharging)
	{
		if ((ManaCurrent - 1.0f) >= 0.0f)
		{
			ManaCurrent -= 1.0f * Delta;
			if (ChargeTimeCurrent < ChargeTimeMaxMax)
			{
				ChargeTimeCurrent += ChargeSpeed * Delta;
				if (ChargeTimeCurrent >= ChargeTimeMaxMid)
				{
					ChargeState = 1;
				}
			}
			else
			{
				if (ChargeTimeCurrent >= ChargeTimeMaxMax)
				{
					ChargeState = 2;
					IsCharging = false;
					IsCharged = true;
					IsChargedEvent(); //Callout to blueprints
					ChargeTimeCurrent = 0.0f;
				}
			}
		}
		else
		{
			kShotReleased = true;
			ShotReleasedEvent(); //Callout to blueprints
			kShotHeld = false;
		}
	}

	if (IsCharging || IsCharged)
	{
		DoChargeColors(Delta);
	}

	attDamage = (attDamageMax + ChargeState);
}

void AcWitch::UpdateStates()
{
	FVector speed = GetVelocity();
	//Any Z velocity is jumping.
	if (speed.Z != 0.0f)
	{
		STATE = JUMP;
	}
	else
	{
		//Any X velocity is running
		if (speed.X != 0.0f)
		{
			STATE = RUN;
		}
		else
		{
			STATE = IDLE;
		}
	}
}

void AcWitch::UpdateAnimations()
{
	FVector speed = GetVelocity();
	
	switch (STATE)
	{
		//Idle
		case 10:
		{
			if (bAttackAnimation)
			{
				DesiredAnim = IdleAttackAnim;
			}
			else
			{
				DesiredAnim = IdleAnim;
			}
			break;
		}
		//Run
		case 11:
		{
			if (bAttackAnimation)
			{
				DesiredAnim = RunAttackAnim;
			}
			else
			{
				DesiredAnim = RunAnim;
			}
			break;
		}
		//Jump.. It's kinda big.
		case 12:
		{
			if (speed.Z < JumpMidVelocity && speed.Z > -JumpMidVelocity)
			{
				if (bAttackAnimation)
				{
					JumpAnim = JumpMidAttackAnim;
				}
				else
				{
					JumpAnim = JumpMidAnim;
				}
			}
			else
			{
				if (speed.Z > 0.0f)
				{
					if (bAttackAnimation)
					{
						JumpAnim = JumpUpAttackAnim;
					}
					else
					{
						JumpAnim = JumpUpAnim;
					}
				}
				else
				{
					if (bAttackAnimation)
					{
						JumpAnim = JumpDownAttackAnim;
					}
					else
					{
						JumpAnim = JumpDownAnim;
					}
				}
			}
			DesiredAnim = JumpAnim;
			break;
		}
	}

	//Sprite Direction
	FVector SpriteScale = Sprite->GetComponentScale();
	SpriteScale.X = Facing;
	if (Sprite->GetComponentScale() != SpriteScale)
	{
		Sprite->SetRelativeScale3D(SpriteScale);
	}

	//Update Sprite center offset and rotational direction
	FVector SpriteOffset = Sprite->RelativeLocation;
	if (Facing == 1)
	{
		SpriteOffset.X = -SpritePosXOffset;
		RotDir = FRotator(0.0f, 0.0f, 0.0f);
	}
	else
	{
		SpriteOffset.X = SpritePosXOffset;
		RotDir = FRotator(0.0f, 180.0f, 0.0f);
	}

	//Apply offset.
	if (Sprite->RelativeLocation != SpriteOffset)
	{
		Sprite->RelativeLocation = SpriteOffset;
	}

	//Apply Rotation
	if (RotComp->RelativeRotation != RotDir)
	{
		RotComp->RelativeRotation = RotDir;
	}

	//Flipbook
	if (Sprite->GetFlipbook() != DesiredAnim)
	{
		Sprite->SetFlipbook(DesiredAnim);
	}

	//Transfer frame from normal to attacking flipbooks positions, or vice versa.
	if (StoredAnimationTime != -1.0f)
	{
		Sprite->SetPlaybackPosition(StoredAnimationTime, false);
		StoredAnimationTime = -1.0f;
	}
}

void AcWitch::UpdatePhysics(int Value)
{
	//0 = normal
	//1 = wall
	if (Value == 0)
	{
		GetCharacterMovement()->GravityScale = BaseGravityScale;
		GetCharacterMovement()->MaxAcceleration = BaseMaxAcceleration;
		GetCharacterMovement()->BrakingFrictionFactor = BaseBrakingFriction;
		GetCharacterMovement()->Mass = BaseMass;
		GetCharacterMovement()->GroundFriction = BaseGroundFriction;
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		GetCharacterMovement()->AirControl = BaseAirControl;
		GetCharacterMovement()->AirControlBoostMultiplier = BaseAirControlBoostMultiplier;
		GetCharacterMovement()->JumpZVelocity = BaseJumpZVelocity;
		GetCharacterMovement()->MaxFlySpeed = BaseMaxFlySpeed;
	}
	else if (Value == 1)
	{
		GetCharacterMovement()->GravityScale = WallGravityScale;
		GetCharacterMovement()->MaxAcceleration = WallMaxAcceleration;
		GetCharacterMovement()->BrakingFrictionFactor = WallBrakingFriction;
		GetCharacterMovement()->Mass = WallMass;
		GetCharacterMovement()->GroundFriction = WallGroundFriction;
		GetCharacterMovement()->MaxWalkSpeed = WallWalkSpeed;
		GetCharacterMovement()->AirControl = WallAirControl;
		GetCharacterMovement()->AirControlBoostMultiplier = WallAirControlBoostMultiplier;
		GetCharacterMovement()->JumpZVelocity = WallJumpZVelocity;
		GetCharacterMovement()->MaxFlySpeed = WallMaxFlySpeed;
	}

}
//Utilities
bool AcWitch::OnGround()
{
	FHitResult Outhit;
	//Start at center point
	FVector start = RotComp->GetComponentLocation();
	//A base to modify end point from
	FVector end = start;
	//Set end point to just below capsule bottom
	end.Z = end.Z - (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + FloorDistance);
	FCollisionQueryParams CollisionParams;

	//Actual trace
	bool IsHit = GetWorld()->LineTraceSingleByChannel(Outhit, start, end, ECC_Visibility, CollisionParams);

	//Draw a debug line, if we should.
	if (bIsDebug)
	{
		DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.5f);
	}

	if (IsHit)
	{
		if (Outhit.bBlockingHit)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool AcWitch::OnWall()
{
	FHitResult Outhit;
	//Start at center point
	FVector start = RotComp->GetComponentLocation();
	//A base to modify end point from
	FVector end = start;
	//Set end point to just below capsule bottom
	end.X = start.X + ((GetCapsuleComponent()->GetScaledCapsuleRadius() + WallDistance) * Facing);

	FCollisionQueryParams CollisionParams;

	//Actual trace
	bool IsHit = GetWorld()->LineTraceSingleByChannel(Outhit, start, end, ECC_Visibility, CollisionParams);

	//Draw a debug line, if we should.
	if (bIsDebug)
	{
		DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.5f);
	}

	if (IsHit)
	{
		if (Outhit.bBlockingHit)
		{
			if (!OnGround())
			{
				if (!bIsOnWall)
				{
					UpdatePhysics(1);
				}
				LaunchCount = LaunchCountMax;
				bIsOnWall = true;
				return true;
			}
			else
			{
				if (bIsOnWall)
				{
					UpdatePhysics(0);
				}
				bIsOnWall = false;
				return false;
			}
		}
		else
		{
			if (bIsOnWall)
			{
				UpdatePhysics(0);
			}
			bIsOnWall = false;
			return false;
		}
	}
	else
	{
		if (bIsOnWall)
		{
			UpdatePhysics(0);
		}
		bIsOnWall = false;
		return false;
	}
}

void AcWitch::DoSetMana(float Inc)
{
	ManaCurrent += Inc;
	if (ManaCurrent > ManaMax)
	{
		ManaCurrent = ManaMax;
	}
}

void AcWitch::DoSetHitpoints(float Inc)
{
	HpCurrent += Inc;
	if (HpCurrent > HpMax)
	{
		HpCurrent = HpMax;
	}
}

void AcWitch::DoRegenShots(int Inc)
{
	ShotCount += Inc;
	if (ShotCount > ShotCountMax)
	{
		ShotCount = ShotCountMax;
	}
}

void AcWitch::DoGetHit(float Damage, FVector OtherLoc)
{
	//Bail out if we shouldn't actually be getting hit.
	if (Damage <= 0.0f)
	{
		return;
	}
	if (!bCanBeHit)
	{
		return;
	}

	//Knockback calculation
	FVector CurrentLoc = GetActorLocation();
	FVector KnockDir = FVector(0.0f, 0.0f, 0.0f);
	if (OtherLoc.X > CurrentLoc.X)
	{
		KnockDir = FVector(-KnockBackPowerX, 0.0f, KnockBackPowerZ);
	}
	else
	{
		KnockDir = FVector(KnockBackPowerX, 0.0f, KnockBackPowerZ);
	}

	//Applied Knockback
	LaunchCharacter(KnockDir, true, true);
	HpCurrent -= Damage;
	if (ChargeState > 0)
	{
		IsCharged = false;
		IsCharging = false;
		ChargeState = 0;
		ShotReleasedEvent();
	}
	if (CurrentSpriteColor != Color_White)
	{
		CurrentSpriteColor = Color_White;
		Sprite->SetSpriteColor(CurrentSpriteColor);
	}
	//Keep Hp within reason
	if (HpCurrent < 0.0f)
	{
		HpCurrent = 0.0f;
	}
	bCanBeHit = false;
}

void AcWitch::DoChargeColors(float Delta)
{
	if (ChargeState == 0)
	{
			Color_ChargeColorShiftSpeedCurrent = Color_ChargeColorShiftSpeed1;
	}
	else if (ChargeState == 1)
	{
			Color_ChargeColorShiftSpeedCurrent = Color_ChargeColorShiftSpeed2;
	}
	else if (ChargeState == 2)
	{
			Color_ChargeColorShiftSpeedCurrent = Color_ChargeColorShiftSpeed3;
	}


	if (ChargeColorState == 0)
	{
		float colorStart = CurrentSpriteColor.G;
		float colorEnd = Color_Pink.G;
		CurrentSpriteColor.G = Approach(colorStart, colorEnd, Color_ChargeColorShiftSpeedCurrent * Delta);
		Sprite->SetSpriteColor(CurrentSpriteColor);
		if (CurrentSpriteColor == Color_Pink)
		{
			ChargeColorState = 1;
		}
	}

	if (ChargeColorState == 1)
	{
		float colorStart = CurrentSpriteColor.G;
		float colorEnd = Color_White.G;
		CurrentSpriteColor.G = Approach(colorStart, colorEnd, Color_ChargeColorShiftSpeedCurrent * Delta);
		Sprite->SetSpriteColor(CurrentSpriteColor);
		if (CurrentSpriteColor == Color_White)
		{
			ChargeColorState = 0;
		}
	}
}

void AcWitch::TimerDelays(float Delta)
{
	//Mana Regeneration
	if (ManaCurrent < ManaMax)
	{
		float TempRegenRate = ManaRegenRate;
		if (ChargeState > 0 ) //IsCharged || IsCharging)
		{
			//ManaRegenDelayCurrent = 0.0f;
			TempRegenRate = 0.0f;
		}
		if (!bAttackAnimation)
		{
			if (TempRegenRate != 0.0f)
			{
				ManaCurrent = Approach(ManaCurrent, ManaMax, TempRegenRate * Delta);
			}
		}
	}

	//Shoot Animation
	if (bAttackAnimationDelayActive)
	{
		if (AttackAnimationDelayCurrent < AttackAnimationDelayMax)
		{
			AttackAnimationDelayCurrent += 1.0f;
		}
		else
		{
			AttackAnimationDelayCurrent = 0.0f;
			bAttackAnimation = false;
			bAttackAnimationDelayActive = false;
		}
	}

	//Reset Firerates
	if (bFireRateDelayActive)
	{
		if (ShotCount >= ShotCountMax)
		{
			FireRateDelayCurrent = 0.0f;
			bFireRateDelayActive = false;
			return;
		}

		if (FireRateDelayMax != FireRate)
		{
			FireRateDelayMax = FireRate;
		}

		if (FireRateDelayCurrent < FireRateDelayMax)
		{
			FireRateDelayCurrent += 1.0f * Delta;
		}
		else
		{
			FireRateDelayCurrent = 0.0f;
			bFireRateDelayActive = false;
			DoRegenShots(1);
		}
	}

	//Hit Animation (Just a visibility toggle, right now)
	if (!bCanBeHit)
	{
		if (HitDelayCurrent < HitDelayMax)
		{
			HitDelayCurrent += 1.0f * Delta;
			Sprite->ToggleVisibility();
		}
		else
		{
			HitDelayCurrent = 0.0f;
			bCanBeHit = true;
			Sprite->SetVisibility(true);
		}
	}
}

float AcWitch::Approach(float start, float end, float shift)
{
	if (start < end)
	{
		return FMath::Min(start + shift, end);
	}
	else
	{
		return FMath::Max(start - shift, end);
	}
}