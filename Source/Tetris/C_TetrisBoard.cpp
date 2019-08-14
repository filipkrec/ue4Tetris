// Fill out your copyright notice in the Description page of Project Settings.

#include "C_TetrisBoard.h"
#include "Runtime/Engine/Classes/Engine/TextRenderActor.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Tetromino.h"
#include "Block.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "TimerManager.h" 
#include "Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
// Sets default values
AC_TetrisBoard::AC_TetrisBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AC_TetrisBoard::BeginPlay() 
{
	Super::BeginPlay();
	world = GetWorld();
	EnableInput(world->GetFirstPlayerController());
	UInputComponent* myInputComp = this->InputComponent;
	if (myInputComp)
	{
		SetupMyPlayerInputComponent(myInputComp);
	}
	SpawnPreviewTetromino();
	SpawnTetromino();
}

void AC_TetrisBoard::SpawnPreviewTetromino()
{
	if (world && previewSpawner)
	{
		FVector spawnLocation = previewSpawner->GetActorLocation();
		FRotator rotator;
		FActorSpawnParameters params;
		params.Owner = this;
		previewTetromino = (world->SpawnActor<ATetromino>(ATetromino::StaticClass(), spawnLocation, rotator, params));
		previewTetromino->Rotate();
		previewTetromino->SetBoard(nullptr);
	}
}

void AC_TetrisBoard::SpawnTetromino()
{
	if (world && spawner)
	{
		int spawnLocationIndex = FMath::RandRange(0, 8);
		FVector spawnLocation = spawner->GetActorLocation();
		spawnLocation.X += 20 * (spawnLocationIndex);
		currentTetromino = previewTetromino;
		currentTetromino->SetPosition(spawnLocationIndex, 0);
		currentTetromino->SetBoard(this);
		currentTetromino->SetActorLocation(spawnLocation);
		currentTetromino->Rotate();
		SpawnPreviewTetromino();
		world->GetTimerManager().SetTimer(dropTimerHandle, this, &AC_TetrisBoard::DropTetromino, 0.2f, true);
	}
}


void AC_TetrisBoard::TetrominoFinalise()
{
	TArray<int> positions = currentTetromino->GetBlockPositions();
	TArray<ABlock*> blocks = currentTetromino->GetBlocks();
	for (int i = 0; i < 4; ++i)
	{
		placedPieces[positions[i * 2]][positions[i * 2 + 1]] = blocks[i];
		placedBooleans[positions[i * 2]][positions[i * 2 + 1]]= true;
	}
	currentTetromino->Destroy();
	points += ClearLines();
	highScore->GetTextRender()->SetText(FText::AsNumber(points));
	SpawnTetromino();
	world->GetTimerManager().UnPauseTimer(dropTimerHandle);
}

void AC_TetrisBoard::DropTetromino()
{
	if (currentTetromino->Move(0, 1) == false)
	{
		world->GetTimerManager().PauseTimer(dropTimerHandle);
		world->GetTimerManager().SetTimer(finaliseTimerHandle, this, &AC_TetrisBoard::TetrominoFinalise, 1.0f, false);
	}
}

int AC_TetrisBoard::ClearLines()
{
	int cleared = -1;
	int j = 0;
	for (int i = 15; i > 0; --i)
	{
		while(placedBooleans[j][i] == true)
			{
			++j;
			if (j == 10) 
				{
				cleared += 1;
				ClearLine(i);
				++i;
				j = 0;
				break;
				}
			}
		j = 0;
	}
	return (cleared >= 0) ? 100 * pow(2, cleared) : 0;
}

void AC_TetrisBoard::ClearLine(int i) {
	for (int j = 0; j < 10; ++j)
	{
		if (placedPieces[j][i] != nullptr) {
			placedBooleans[j][i] = false;
			placedPieces[j][i]->Destroy();
			placedPieces[j][i] = NULL;
		}
		for (int row = i; row > 0; --row)
		{
			if (placedBooleans[j][row - 1] == true)
			{
				placedPieces[j][row - 1]->MoveDown();
				placedPieces[j][row] = placedPieces[j][row - 1];
				placedBooleans[j][row - 1] = false;
				placedBooleans[j][row] = true;
			}
		}
	}
}

bool AC_TetrisBoard::CheckPositions(TArray<int> positionsToCheck)
{
	for(int i = 0; i <4;++i)
	{
		if (placedBooleans[i * 2][i * 2 + 1] == true)
			return true;
	}
	return false;
}

void AC_TetrisBoard::MoveTetrominoRight()
{
	currentTetromino->Move(1, 0);
	world->GetTimerManager().ClearTimer(finaliseTimerHandle);
	world->GetTimerManager().SetTimer(prolongTimerHandle, this, &AC_TetrisBoard::ProlongTimer, 0.3f, false);
}

void AC_TetrisBoard::MoveTetrominoLeft()
{
	currentTetromino->Move(-1, 0);
	world->GetTimerManager().ClearTimer(finaliseTimerHandle);
	world->GetTimerManager().SetTimer(prolongTimerHandle, this, &AC_TetrisBoard::ProlongTimer, 0.3f, false);
}	


void AC_TetrisBoard::RotateTetromino()
{
	currentTetromino->Rotate();
	world->GetTimerManager().ClearTimer(finaliseTimerHandle);
	world->GetTimerManager().SetTimer(prolongTimerHandle, this, &AC_TetrisBoard::ProlongTimer, 0.3f, false);
}

void AC_TetrisBoard::ProlongTimer()
{
	world->GetTimerManager().UnPauseTimer(dropTimerHandle);
}

void AC_TetrisBoard::SetupMyPlayerInputComponent(UInputComponent* myInputComponent)
{
	myInputComponent->BindAction("LeftMove", IE_Pressed, this, &AC_TetrisBoard::MoveTetrominoLeft);
	myInputComponent->BindAction("RightMove", IE_Pressed, this, &AC_TetrisBoard::MoveTetrominoRight);
	myInputComponent->BindAction("Rotate", IE_Pressed, this, &AC_TetrisBoard::RotateTetromino);
};

// Called every frame
void AC_TetrisBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}




