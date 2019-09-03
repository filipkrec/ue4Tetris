// Fill out your copyright notice in the Description page of Project Settings.

#include "C_TetrisBoard.h"
#include "Runtime/Engine/Classes/Engine/TextRenderActor.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Tetromino.h"
#include "Block.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "TimerManager.h" 
#include "Save.h"
#include "PlayerSave.h"
#include "Kismet/GameplayStatics.h"
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
	paused = false;
	level = 1;
	dropSpeed = 0.7f;
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

void AC_TetrisBoard::EndGame()
{
	world->GetTimerManager().ClearTimer(dropTimerHandle);
	world->GetTimerManager().ClearTimer(finaliseTimerHandle);
	world->GetTimerManager().ClearTimer(prolongTimerHandle);
	TMap<FString, int> HighScores;
	FString playerName;

	if (UGameplayStatics::DoesSaveGameExist("Player", 0)) {
		UPlayerSave* LoadGameInstancePlayer = Cast<UPlayerSave>(UGameplayStatics::CreateSaveGameObject(USave::StaticClass()));
		LoadGameInstancePlayer = Cast<UPlayerSave>(UGameplayStatics::LoadGameFromSlot("Player", 0));
		playerName = LoadGameInstancePlayer->playerName;
	}
	else playerName = "AAA";

	if (UGameplayStatics::DoesSaveGameExist("Save", 0)) {
		USave* LoadGameInstance = Cast<USave>(UGameplayStatics::CreateSaveGameObject(USave::StaticClass()));
		LoadGameInstance = Cast<USave>(UGameplayStatics::LoadGameFromSlot("Save", 0));
		HighScores = LoadGameInstance->HighScores;
	}
	int* oldScore = HighScores.Find(playerName);
	if (oldScore != nullptr)
	{
		if (*oldScore < points)
			HighScores.Add(playerName, points);

	}
	else
		HighScores.Add(playerName, points);

	USave* SaveGameInstance = Cast<USave>(UGameplayStatics::CreateSaveGameObject(USave::StaticClass()));
	SaveGameInstance->HighScores = HighScores;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, "Save", 0);

	screenText->GetTextRender()->SetText(FText::Format(FText::FromStringTable("StringTableCSV", "GameOver"), points));
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
		previewTetromino->RotateAndPosition();
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
		currentTetromino->RotateAndPosition();
		SpawnPreviewTetromino();
		world->GetTimerManager().SetTimer(dropTimerHandle, this, &AC_TetrisBoard::MoveTetrominoDown, dropSpeed, true);
	}
}


void AC_TetrisBoard::TetrominoFinalise()
{
	TArray<int> positions = currentTetromino->GetBlockPositions();
    isOver = false;
	for (int i = 0; i < 4; ++i)
	{
		if (positions[i * 2 + 1] == 0)
			isOver = true;
	}
	if (isOver == false) {
		TArray<ABlock*> blocks = currentTetromino->GetBlocks();
		for (int i = 0; i < 4; ++i)
		{
			placedPieces[positions[i * 2]][positions[i * 2 + 1]] = blocks[i];
			placedBooleans[positions[i * 2]][positions[i * 2 + 1]] = true;
		}
		currentTetromino->Destroy();
		points += ClearLines();
		highScore->GetTextRender()->SetText(FText::AsNumber(points));
		LevelUp();
		SpawnTetromino();
	}
	else EndGame();
}

void AC_TetrisBoard::LevelUp()
{
	if (points > (level * (700 + 100 * level)))
	{
		level += 1;
		if (dropSpeed > 0.1)
			dropSpeed = dropSpeed * (0.8);
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
		int col = positionsToCheck[i * 2];
		int row = positionsToCheck[i * 2 + 1];
		if (placedBooleans[col][row] == true || col < 0 || col > 9 || row > 14) 
			return false;
	}
	return true;
}

void AC_TetrisBoard::StartMoveLeft()
{
	MoveTetrominoLeft();
	world->GetTimerManager().SetTimer(moveTimerHandle, this, &AC_TetrisBoard::MoveTetrominoLeft, 0.1F, true);
}

void AC_TetrisBoard::StartMoveRight()
{
	MoveTetrominoRight();
	world->GetTimerManager().SetTimer(moveTimerHandle, this, &AC_TetrisBoard::MoveTetrominoRight, 0.1F, true);
}

void AC_TetrisBoard::StopMove()
{
	world->GetTimerManager().PauseTimer(moveTimerHandle);
	world->GetTimerManager().ClearTimer(moveTimerHandle);
}

void AC_TetrisBoard::MoveTetrominoDown()
{
	if (currentTetromino->Move(0, 1) == false)
	{
		world->GetTimerManager().PauseTimer(dropTimerHandle);
		world->GetTimerManager().SetTimer(finaliseTimerHandle, this, &AC_TetrisBoard::TetrominoFinalise, dropSpeed, false);
	}
}

void AC_TetrisBoard::DropTetromino()
{
	while (currentTetromino->Move(0, 1) == true)
	{
	};
	world->GetTimerManager().PauseTimer(dropTimerHandle);
	world->GetTimerManager().SetTimer(finaliseTimerHandle, this, &AC_TetrisBoard::TetrominoFinalise, dropSpeed, false);
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
	currentTetromino->RotateAndPosition();
	world->GetTimerManager().ClearTimer(finaliseTimerHandle);
	world->GetTimerManager().SetTimer(prolongTimerHandle, this, &AC_TetrisBoard::ProlongTimer, 0.3f, false);
}

void AC_TetrisBoard::ProlongTimer()
{
	world->GetTimerManager().UnPauseTimer(dropTimerHandle);
}


void AC_TetrisBoard::Restart()
{
	if(isOver == true)
	UGameplayStatics::OpenLevel(this, FName(*world->GetName()), true);
}


void AC_TetrisBoard::Exit()
{
	if(isOver == true)
	UGameplayStatics::OpenLevel(this, "MainMenu", true);
}

void AC_TetrisBoard::Pause()
{
	if (paused == false) {
		world->GetTimerManager().PauseTimer(dropTimerHandle);
		world->GetTimerManager().PauseTimer(finaliseTimerHandle);
		world->GetTimerManager().PauseTimer(prolongTimerHandle);
		screenText->GetTextRender()->SetText(FText::Format(FText::FromStringTable("StringTableCSV", "Paused"), level));
		paused = true;
	}
	else {
		world->GetTimerManager().UnPauseTimer(dropTimerHandle);
		world->GetTimerManager().UnPauseTimer(finaliseTimerHandle);
		world->GetTimerManager().UnPauseTimer(prolongTimerHandle);
		screenText->GetTextRender()->SetText("");
		paused = false;
	}
}

void AC_TetrisBoard::SetupMyPlayerInputComponent(UInputComponent* myInputComponent)
{
	myInputComponent->BindAction("LeftMove", IE_Pressed, this, &AC_TetrisBoard::StartMoveLeft);
	myInputComponent->BindAction("LeftMove", IE_Released, this, &AC_TetrisBoard::StopMove);
	myInputComponent->BindAction("RightMove", IE_Pressed, this, &AC_TetrisBoard::StartMoveRight);
	myInputComponent->BindAction("RightMove", IE_Released, this, &AC_TetrisBoard::StopMove);
	myInputComponent->BindAction("Rotate", IE_Pressed, this, &AC_TetrisBoard::RotateTetromino);
	myInputComponent->BindAction("Drop", IE_Pressed, this, &AC_TetrisBoard::DropTetromino);
	myInputComponent->BindAction("Restart", IE_Pressed, this, &AC_TetrisBoard::Restart);
	myInputComponent->BindAction("Escape", IE_Pressed, this, &AC_TetrisBoard::Exit);
	myInputComponent->BindAction("Pause", IE_Pressed, this, &AC_TetrisBoard::Pause);
};

// Called every frame
void AC_TetrisBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}




