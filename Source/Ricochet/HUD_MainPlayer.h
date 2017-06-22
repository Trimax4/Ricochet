// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUD_MainPlayer.generated.h"

/**
 * 
 */
UCLASS()
class RICOCHET_API AHUD_MainPlayer : public AHUD
{
	GENERATED_BODY()
	
	AHUD_MainPlayer(const FObjectInitializer& ObjectInitializer);

	/** Primary draw call for the HUD */
	virtual void DrawHUD();

private:
	/** Crosshair asset pointer */
	UTexture2D* CrosshairTex;

};
