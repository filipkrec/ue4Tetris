// Fill out your copyright notice in the Description page of Project Settings.


#include "Tetromino.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Block.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ATetromino::ATetromino()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ATetromino::BeginPlay()
{
	Super::BeginPlay();
	GenerateTetromino();
}

void ATetromino::GenerateTetromino()
{
	type = TetrominoType(FMath::RandRange(1, 7));
	rotation = 0;
	UWorld* world = GetWorld();
	FRotator rotator(0, 0, 0);
	FActorSpawnParameters params;
	params.Owner = this;
	TSubclassOf<class ABlock> ToSpawn;
	for (int i = 0; i < 4; ++i) {
		blocks[i] = world->SpawnActor<ABlock>(ABlock::StaticClass(), GetActorLocation(), rotator, params);
	}
}

void ATetromino::Rotate()
{
	if (rotation == 4)
		rotation = 1;
	else rotation += 1;

	switch (type) {
	case line:
		GenerateLine();
		break;
	case block:
		GenerateBlock();
		break;
	case T:
		GenerateT();
		break;
	case L:
		GenerateL();
		break;
	case LR:
		GenerateLReverse();
		break;
	case squigly:
		GenerateSquigly();
		break;
	case squiglyR:
		GenerateSquiglyReverse();
		break;
	}
	SetBlocks();
}

TArray<int> ATetromino::GetBlockPositions()
{
	TArray<int> blockPositions;
	for (int i = 0; i < 4; ++i)
	{
		blockPositions.Add(currentColumn + blocks[i]->GetColumnDifference());
		blockPositions.Add(currentRow + blocks[i]->GetRowDifference());
	}
	return blockPositions;
}

bool ATetromino::SetBlockDifferences(TArray<int> newBlockPositions)
{
	if (CheckBlocks(newBlockPositions)) {
		for (int i = 0; i < 4; ++i)
		{
			blocks[i]->SetDifference(newBlockPositions[i * 2], newBlockPositions[i * 2 + 1]);
		}
		return true;
	}
	if (rotation == 1)
		rotation = 4;
	else rotation -= 1;

	return false;
}

bool ATetromino::CheckBlocksMove(int columnsToMove, int rowsToMove)
{
	TArray<int> positions = GetBlockPositions();
	for (int i = 0; i < 4; ++i)
	{
		positions[i * 2] += columnsToMove;
		positions[i * 2 + 1] += rowsToMove;
	}
	return board->CheckPositions(positions);
}

bool ATetromino::CheckBlocks(TArray<int> positionDifferences)
{
	if (board != nullptr)
	{
		TArray<int> positions = positionDifferences;
		for (int i = 0; i < 4; ++i)
		{
			positions[i * 2] += currentColumn;
			positions[i * 2 + 1] += currentRow;
		}
		return board->CheckPositions(positions);
	}
	return true;
}

bool ATetromino::Move(int columnsToMove, int rowsToMove)
{
	if (CheckBlocksMove(columnsToMove, rowsToMove)) {
		currentColumn += columnsToMove;
		currentRow += rowsToMove;
		FVector location = GetActorLocation();
		location.X += 20 * columnsToMove;
		location.Z -= 20 * rowsToMove;
		SetActorLocation(location);
		SetBlocks();
		return true;
	}
	return false;
}

void ATetromino::SetBlocks()
{
	for (int i = 0; i < 4; ++i) {
		blocks[i]->SetLocation(GetActorLocation());
	}
}

void ATetromino::GenerateBlock()
{
	TArray<int> differences = { 0, 0, 0, 1, 1, 0, 1, 1 };
	SetBlockDifferences(differences);
}

void ATetromino::GenerateL()
{
	TArray<int> differences;
	if (rotation == 1)
		differences = { 0, 0, 1, 0, 1, 1, 1, 2 };
	else if (rotation == 2)
		differences = { 2,0,2,1,1,1,0,1 };
	else if (rotation == 3)
		differences = { 1,2,0,2,0,1,0,0 };
	else if (rotation == 4)
		differences = {0, 1, 0, 0, 1, 0, 2, 0 };
	SetBlockDifferences(differences);


}

void ATetromino::GenerateLReverse()
{
	TArray<int> differences;
	if (rotation == 1)
		differences = { 0, 0, 1, 0, 0, 1, 0, 2 };
	else if (rotation == 2)
		differences = { 0, 0, 1, 0, 2, 0, 2, 1 };
	else if (rotation == 3)
		differences = { 0,2,1,2,1,1,1,0 };
	else if (rotation == 4)
		differences = { 0,0,0,1,1,1,2,1 };

	SetBlockDifferences(differences);
}

void ATetromino::GenerateLine()
{
	TArray<int> differences;
	if (rotation == 1 || rotation == 3)
		differences = { 0, 0, 0, 1, 0, 2, 0, 3 };
	else
		differences = {0, 0, 1, 0, 2, 0, 3, 0 };

	SetBlockDifferences(differences);
}

void ATetromino::GenerateSquigly()
{
	TArray<int> differences;
	if (rotation == 1 || rotation == 3)
		differences = { 0, 0, 0, 1, 1, 1, 1, 2 };
	else
		differences = {2,0,1,0,1,1,0,1 };

	SetBlockDifferences(differences);
}

void ATetromino::GenerateSquiglyReverse()
{
	TArray<int> differences;
	if (rotation == 1 || rotation == 3)
		differences = { 1, 0, 1, 1, 0, 1, 0, 2 };
	else
		differences = {2,1,1,1,1,0,0,0 };

	SetBlockDifferences(differences);

}

void ATetromino::GenerateT()
{
	TArray<int> differences;
	if (rotation == 1)
		differences = { 0, 0, 1, 0, 2, 0, 1, 1 };
	else if (rotation == 2)
		differences = { 1,0,1,1,0,1,1,2 };
	else if (rotation == 3)
		differences = { 1,0,0,1,1,1,2,1 };
	else if (rotation == 4)
		differences = { 1,0,1,1,1,2,2,1 };

	SetBlockDifferences(differences);
}

ATetromino::ATetromino(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);
}

void ATetromino::SetPosition(int column, int row) {
	currentColumn = column;
	currentRow = row;
}

void ATetromino::SetBoard(AC_TetrisBoard* fatherBoard) {
	board = fatherBoard;
}

int ATetromino::GetCurrentRow() {
	return currentRow;
}
int ATetromino::GetCurrentColumn() {
	return currentColumn;
}
TArray<ABlock*> ATetromino::GetBlocks() {
	TArray<ABlock*> toReturn;
	for(int i=0;i<4;++i)
	toReturn.Add(blocks[i]);

	return toReturn;
}

// Called every frame
void ATetromino::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

