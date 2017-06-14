// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PracticeMode.generated.h"

/**
 * 
 */
UCLASS()
class RICOCHET_API APracticeMode : public AGameModeBase
{
	GENERATED_BODY()
	
	virtual void StartPlay() override;
	
	APracticeMode(const FObjectInitializer& ObjectInitializer);
	
};
