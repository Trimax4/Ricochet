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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	//handles moving forward/backward
	UFUNCTION()
	void MoveZ(float Val);
	//handles strafing
	UFUNCTION()
	void MoveX(float Val);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
