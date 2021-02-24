// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"
#include "cPickupParent.generated.h"

UCLASS()
class WITCHGAME_API AcPickupParent : public AActor
{
	GENERATED_BODY()

		// Name of the Sprite component
		static FName SpriteComponentName;

public:

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* Sprite;

	/** Pickup Radious */
	UPROPERTY(VisibleAnywhere, Category = Projectile)
	class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		float PickupSpeed1 = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		float PickupSpeed2 = 1.0f;

	AActor* TargetActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		bool bHasTriggeredSeeking = false;

	// Timers
	float SpeedPickupDelayCurrent = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timers)
		float SpeedPickupDelayMax = 0.5f;
	bool bSpeedPickupDelayActive = false;





protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(Blueprintcallable)
		void StartSeekingEvent();
	UFUNCTION(Blueprintcallable)
		void DoSeeking(float Delta);
	void DelayTimers(float Delta);

	// Sets default values for this actor's properties
	AcPickupParent();

	FORCEINLINE class UPaperFlipbookComponent* GetSprite() const { return Sprite; }
	FORCEINLINE class USphereComponent* GetSphere() const { return CollisionComp; }
	FORCEINLINE class UProjectileMovementComponent* GetMove() const { return ProjectileMovement; }

};
