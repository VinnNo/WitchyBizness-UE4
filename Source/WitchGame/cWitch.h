// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/Character.h"
#include "cWitch.generated.h"


UCLASS()
class /*WITCHGAME_API*/ AcWitch : public ACharacter
{
	GENERATED_UCLASS_BODY()

		// Name of the Sprite component
		static FName SpriteComponentName;

public:
	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* Sprite;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* CamComp;


	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* RotComp;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* ShotPosComp;

	//Sprite Variables-----------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* IdleAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* RunAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* JumpAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* JumpUpAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* JumpMidAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* JumpDownAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* IdleAttackAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* RunAttackAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* JumpUpAttackAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* JumpMidAttackAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* JumpDownAttackAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animations)
		class UPaperFlipbook* DesiredAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animations)
		class UPaperFlipbook* StoredAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animations)
		float StoredAnimationTime = -1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animations)
		int StoredAnimationFrame = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		float JumpMidVelocity = 120.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		FRotator RotDir = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		float SpritePosXOffset = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		bool bAttackAnimation = false;


	//End Sprite Vars-----------------------------------------------


	//Skills----------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		bool bHasSkill_DoubleJump = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		float LaunchHeight = 700;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		int LaunchCountMax = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		int LaunchCount = LaunchCountMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		bool bHasSkill_WallJump = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		float WallPushOff = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		int ShotCountMax = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		int ShotCount = ShotCountMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		float ManaMax = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		float ManaCurrent = ManaMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		float HpMax = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		float HpCurrent = HpMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		float ShotCost = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		float FireRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		float ManaRegenRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		float KnockBackPowerX = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		float KnockBackPowerZ = 350.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		float attDamageMax = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		float attDamage = attDamageMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		bool bCanBeHit = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		bool IsCharged = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		bool IsCharging = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		float ChargeTimeCurrent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		float ChargeTimeMaxMid = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		float ChargeTimeMaxMax = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		float ChargeSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		int ChargeState = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trace)
		float FloorDistance = 2.22f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trace)
		float WallDistance = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trace)
		bool bIsOnWall = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
		bool bIsDebug = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
		bool isTouchScreen = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		float InputThreshold = 0.4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		bool kShotHeld = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		bool kShotReleased = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Colors)
		FLinearColor Color_Charge1 = FLinearColor(1.0f, 0.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Colors)
		FLinearColor Color_Charge2 = FLinearColor(1.0f, 0.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Colors)
		float Color_ChargeColorShiftSpeed1 = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Colors)
		float Color_ChargeColorShiftSpeed2 = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Colors)
		float Color_ChargeColorShiftSpeed3 = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Colors)
		float Color_ChargeColorShiftSpeedCurrent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Colors)
		FLinearColor Color_White = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Colors)
		FLinearColor Color_Pink = FLinearColor(1.0f, 0.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Colors)
		FLinearColor CurrentSpriteColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Colors)
		int ChargeColorState = 0;

	//Timers-----------------------
	float ManaRegenDelayCurrent = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timers)
		float ManaRegenDelayMax = 1.0f;

		float AttackAnimationDelayCurrent = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timers)
		float AttackAnimationDelayMax = 10.0f;
	bool bAttackAnimationDelayActive = false;

	float FireRateDelayCurrent = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timers)
		float FireRateDelayMax = FireRate;
	bool bFireRateDelayActive = false;

	float HitDelayCurrent = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timers)
		float HitDelayMax = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timers)
	float StoredDelta = 0.0f;

	//EndSkills
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillsExt)
		float PickupHealingMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillsExt)
		float PickupMoneyMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillsExt)
		float MoneyCount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillsExt)
		float LuckNormal = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillsExt)
		float LuckMana = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillsExt)
		float LuckHitPoints = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float MaxFallSpeed = 0.0f;

	//Pickups
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickups)
		int PickupCount = 0;
	

	//State-------------------------------
	const int IDLE = 10;
	const int RUN = 11;
	const int JUMP = 12;

	int STATE = IDLE;
	bool bNewState = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = States)
	int Facing = 1;


	//Movement methods
	//Base
	float BaseGravityScale = 1.75f;
	float BaseMaxAcceleration = 512.0f;
	float BaseBrakingFriction = 1.0f;
	float BaseMass = 80.0f;
	float BaseGroundFriction = 9.5f;
	float BaseWalkSpeed = 300.0f;
	float BaseAirControl = 0.80f;
	float BaseAirControlBoostMultiplier = 1.25f;
	float BaseJumpZVelocity = 1000.f;
	float BaseMaxFlySpeed = 600.0f;

	//Wall
	float WallGravityScale = 1.75f;
	float WallMaxAcceleration = 512.0f;
	float WallBrakingFriction = 1.0f;
	float WallMass = 50.0f;
	float WallGroundFriction = 9.5f;
	float WallWalkSpeed = 300.0f;
	float WallAirControl = 0.80f;
	float WallAirControlBoostMultiplier = 1.25f;
	float WallJumpZVelocity = 500.f;
	float WallMaxFlySpeed = 600.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Landed(const FHitResult& Hit);
	void DoLanded();
	UFUNCTION(Blueprintcallable)
	void DoJumped();
	UFUNCTION(Blueprintcallable)
	void DoStopJump();
	UFUNCTION(Blueprintcallable)
	void DoShotPressed();
	UFUNCTION(Blueprintcallable)
	void DoShotReleased();
	void DoCharge(float Delta);
	void DoChargeColors(float Delta);
	void UpdateStates();
	void UpdateAnimations();
	void TimerDelays(float Delta);
	UFUNCTION(Blueprintcallable)
	void HorizontalMovement(float Value);
	void DoSetMovement(float Value);
	void UpdatePhysics(int Value);
	bool OnGround();
	bool OnWall();
	float Approach(float start, float end, float shift);

	UFUNCTION(BlueprintImplementableEvent)
		void MakeShotEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void OnDoubleJumped();
	UFUNCTION(BlueprintImplementableEvent)
		void IsChargingEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void IsChargedEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void ShotReleasedEvent();

	UFUNCTION(BlueprintCallable)
		void DoRegenShots(int Inc);

	UFUNCTION(BlueprintCallable)
		void DoSetMana(float Inc);

	UFUNCTION(BlueprintCallable)
		void DoSetHitpoints(float Inc);

	UFUNCTION(BlueprintCallable)
		void DoGetHit(float Damage, FVector OtherLoc);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Sets default values for this character's properties
	AcWitch();


	/** Returns Sprite subobject **/
	FORCEINLINE class UPaperFlipbookComponent* GetSprite() const { return Sprite; }
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	FORCEINLINE class UCameraComponent* GetCam() const { return CamComp; }
	FORCEINLINE class USceneComponent* GetRot() const { return RotComp; }
	FORCEINLINE class UArrowComponent* GetShotComp() const { return ShotPosComp; }


};