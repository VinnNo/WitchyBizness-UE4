
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "cEnemyParent.h"
#include "cEnemyWalkerBase.generated.h"

/**
 *
 */
UCLASS()
class WITCHGAME_API AcEnemyWalkerBase : public AcEnemyParent
{
	GENERATED_BODY()

		UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* EyesSprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skills, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* RotationalComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skills, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* TracerComp;
public:
	AcEnemyWalkerBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* IdleAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* RunAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* JumpAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* HitAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* Trans1Anim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* Trans2Anim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* Trans3Anim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* Trans4Anim;

	class UPaperFlipbook* DesiredAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* IdleEyesAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* RunEyesAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* HitEyesAnim;

	class UPaperFlipbook* DesiredEyesAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		FRotator RotCompDir = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		int Facing = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tracing)
		float FloorDistance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tracing)
		float TraceDistance = 0.0f;

	//**************( Timers )**************//
	float IdleDelayCurrent = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timers)
		float IdleDelayMax = 10.0f;
	bool bIdleDelayActive = false;

	float HitDelayCurrent = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timers)
		float HitDelayMax = 10.0f;
	bool bHitDelayActive = false;

	float TurnDelayCurrent = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timers)
		float TurnDelayMax = 10.0f;
	bool bTurnDelayActive = false;
	bool bCanTurn = true;

	//**************( Functions )**************//

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			FVector NormalImpulse,
			const FHitResult& Hit);

	void RunFloorTrace();
	void RunWallTrace();
	void OnGround();
	void MoveH();
	void UpdateStates();
	void RunNewStates();
	void UpdateAnimation();
	void ChangeDirection();
	void DelayTimers(float Delta);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns Rotational subobject **/
	FORCEINLINE class USceneComponent* GetRotationalComponent() const { return RotationalComponent; }
	/** Returns Tracer subobject **/
	FORCEINLINE class USceneComponent* GetTracerComp() const { return TracerComp; }
	/** Returns Eyes subobject **/
	FORCEINLINE class UPaperFlipbookComponent* GetEyesSprite() const { return EyesSprite; }
};
