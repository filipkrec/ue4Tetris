#pragma once
#include "Runtime/Engine/Classes/Engine/TextRenderActor.h"
#include "Save.h"
#include "Kismet/GameplayStatics.h"
#include "Block.h"
#include "Tetromino.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_TetrisBoard.generated.h"
class ATetromino; class ABlock; class ATextRenderActor;
UCLASS()
class TETRIS_API AC_TetrisBoard : public AActor {	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_TetrisBoard();

	UPROPERTY(EditAnywhere)
		AActor* spawner;

	UPROPERTY(EditAnywhere)
		AActor* previewSpawner;

	UPROPERTY(EditAnywhere)
		ATextRenderActor* highScore;

	UPROPERTY(EditAnywhere)
		ATextRenderActor* screenText;

	
	virtual bool CheckPositions(TArray<int> positionsToCheck);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndGame();

private:
	virtual void SetupMyPlayerInputComponent(UInputComponent* myInputComponent);
	virtual void MoveTetrominoLeft();
	virtual void MoveTetrominoRight();
	virtual void RotateTetromino();
	virtual void TetrominoFinalise();
	virtual void ProlongTimer();
	virtual void Pause();
	int ClearLines();
	virtual void ClearLine(int i);
	virtual void SpawnTetromino();
	virtual void MoveTetrominoDown();
	virtual void SpawnPreviewTetromino();
	virtual void DropTetromino();
	virtual void LevelUp();
	virtual void Restart();
	virtual void Exit();


	int level;
	float dropSpeed;
	bool paused;
	bool isOver;
	ABlock* placedPieces[10][15] = { {NULL} };
	bool placedBooleans[10][15] = { {false} };
	ATetromino* currentTetromino;
	ATetromino* previewTetromino;
	FTimerHandle dropTimerHandle;
	FTimerHandle finaliseTimerHandle;
	FTimerHandle prolongTimerHandle;
	UWorld* world;
	int points = 0;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
