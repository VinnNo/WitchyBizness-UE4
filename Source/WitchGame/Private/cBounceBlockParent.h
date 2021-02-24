// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/Actor.h"
#include "cBounceBlockParent.generated.h"

UCLASS()
class AcBounceBlockParent : public AActor
{
	GENERATED_BODY()

	// Name of the Sprite component
	static FName SpriteComponentName;
	
public:

	//Flipbook
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* Sprite;

	UPROPERTY(VisibleAnywhere, Category = Collision)
		class UBoxComponent* CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	AActor* TargetActor = nullptr;

	//AcWitch* WitchActor = Cast<AcWitch>(WitchActor);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
		float LaunchStrength = 1300.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
		void DoOverlap();

	// Sets default values for this actor's properties
	AcBounceBlockParent();

	FORCEINLINE class UPaperFlipbookComponent* GetSprite() const { return Sprite; }
	FORCEINLINE class UBoxComponent* GetBox() const { return CollisionComp; }
};
