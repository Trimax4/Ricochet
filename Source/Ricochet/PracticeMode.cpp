// Fill out your copyright notice in the Description page of Project Settings.

#include "PracticeMode.h"
#include "MainPlayer.h"
#include "Engine.h"

APracticeMode::APracticeMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = AMainPlayer::StaticClass();
}

void APracticeMode::StartPlay()
{
	Super::StartPlay();

	//StartMatch();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("HELLO WORLD"));
	}
}


