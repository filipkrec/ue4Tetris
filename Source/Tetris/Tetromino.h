// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "C_TetrisBoard.h"
#include "Block.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tetromino.generated.h"
class ABlock; class AC_TetrisBoard;
UCLASS()
class TETRIS_API ATetromino : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATetromino();
	virtual int GetCurrentRow();
	virtual int GetCurrentColumn();
	virtual void SetBoard(AC_TetrisBoard* fatherBoard);
	virtual void SetPosition(int column, int row);
	virtual TArray<int> GetBlockPositions();
	virtual TArray<ABlock*> GetBlocks();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	enum TetrominoType { block = 1, line = 2, T = 3, L = 4, LR = 5, squigly = 6, squiglyR = 7};
	AC_TetrisBoard* board;
	int currentRow;
	int currentColumn;
	int rotation;
	TetrominoType type;
	ABlock* blocks[4];
	virtual bool SetBlockDifferences(TArray<int> newBlockPositions);
	virtual void GenerateTetromino();
	virtual void GenerateBlock();
	virtual void GenerateLine();
	virtual void GenerateL();
	virtual void GenerateLReverse();
	virtual void GenerateT();
	virtual void GenerateSquigly();
	virtual void GenerateSquiglyReverse();
	virtual bool CheckBlocks(TArray<int> positionDifferences);
	virtual bool CheckBlocksMove(int columnsToMove, int rowsToMove);
	virtual void SetBlocks();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool Move(int columnsToMove, int rowsToMove);
	virtual void Rotate();

	ATetromino(const FObjectInitializer& ObjectInitializer);
	UPROPERTY(VisibleAnywhere, Category = "Mesh") 
		UStaticMeshComponent* BaseMesh;
};
