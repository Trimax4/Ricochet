// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"


UCLASS()
class RICOCHET_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()
	

public:
	// Sets default values for this character's properties
	AMainPlayer();

	// Constructor for AFPSCharacter
	AMainPlayer(const FObjectInitializer& ObjectInitializer);

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FirstPersonMesh;

	// Gun muzzle's offset from the camera location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AFireball> ProjectileClass;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	//handles moving forward/backward
	UFUNCTION()
	void MoveZ(float Val);
	//handles strafing
	UFUNCTION()
	void MoveX(float Val);

	//sets jump flag when key is pressed
	UFUNCTION()
	void OnStartJump();
	//clears jump flag when key is released
	UFUNCTION()
	void OnStopJump();
	
	// Function that handles firing projectiles.
	UFUNCTION()
	void LeftClick();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
