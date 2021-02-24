// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/Character.h"
#include "cEnemyParent.generated.h"

UCLASS()
class WITCHGAME_API AcEnemyParent : public ACharacter
{
	GENERATED_BODY()

		// Name of the Sprite component
		static FName SpriteComponentName;




public:

	//********************( Variables )******************************//

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* Sprite;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats, meta = (AllowPrivateAccess = "false"))
		float HpMax = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats, meta = (AllowPrivateAccess = "false"))
		float HpCurrent = HpMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats, meta = (AllowPrivateAccess = "false"))
		float contactDamageMax = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats, meta = (AllowPrivateAccess = "false"))
		float contactDamageCurrent = contactDamageMax;
	bool bShouldDie = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		float SpriteDrawOffset = 7.50f;

	//************************( States )*************************//
	const int IDLE = 10;
	const int RUN = 11;
	const int JUMP = 12;
	const int HIT = 13;
	const int TRA1 = 14;
	const int TRA2 = 15;
	const int TRA3 = 16;
	const int TRA4 = 17;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = States)
		int STATE = IDLE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = States)
		bool bNewState = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = States)
		bool bShouldDoTurn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
		bool bIsDebug = false;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		virtual void GetHit(float InDamage);

	//UFUNCTION(Blueprintcallable)
		//virtual void DoTurnAround();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this character's properties
	AcEnemyParent();

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns Sprite subobject **/
	FORCEINLINE class UPaperFlipbookComponent* GetSprite() const { return Sprite; }

};
