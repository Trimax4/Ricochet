// Fill out your copyright notice in the Description page of Project Settings.

#include "PracticeMode.h"
//#include "MainPlayer.h"
#include "Engine.h"

APracticeMode::APracticeMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnObject(TEXT("Pawn'/Game/MainPlayer_BP.MainPlayer_BP_C'"));
	if (PlayerPawnObject.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnObject.Class;
	}
}

void APracticeMode::StartPlay()
{
	Super::StartPlay();


	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("HELLO WORLD"));
	}
}


