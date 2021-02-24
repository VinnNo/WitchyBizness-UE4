// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "cPlayerShot.generated.h"

UCLASS()
class WITCHGAME_API AcPlayerShot : public AActor
{
	GENERATED_BODY()

		// Name of the Sprite component
		static FName SpriteComponentName;
	
public:	

	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* Sprite;

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

	// Sets default values for this actor's properties
	AcPlayerShot();

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
	UFUNCTION(BlueprintImplementableEvent)
		void OnDeath();

	void DelayTimers(float Delta);

	void FlipGravity();

	UFUNCTION(BlueprintImplementableEvent)
	void DebugPrinter();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
		bool bIsDebug = false;

	float LifeTimeDelayCurrent = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timers)
		float LifeTimeDelayMax = 3.0f;
	bool bLifeTimeDelayActive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		float attDamage = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		float ShotAccuracy = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		float ShotAdjustFrequency = 0.0f;

	FVector ShotVelAdjust = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timers)
		float DestroyDelay = 0.035f;





protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};
