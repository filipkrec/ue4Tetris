// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Save.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API USave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
	TMap<FString,int> HighScores;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual FString GetScoresString();

	USave();

};
