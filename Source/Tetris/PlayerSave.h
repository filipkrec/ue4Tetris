// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSave.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API UPlayerSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString playerName;

	UPlayerSave();

	virtual void SetPlayerName(FString name);	
};
