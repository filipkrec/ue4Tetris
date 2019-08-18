// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Tetromino.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"

UCLASS()
class TETRIS_API ABlock : public AActor
{
	GENERATED_BODY()
	
public:	
	enum BlockColor { yellow = 1, blue = 2, purple = 3, red = 4, orange = 5, green = 6, darkBlue = 7 };
	// Sets default values for this actor's properties
	ABlock();
	//initializes object mesh
	ABlock(const FObjectInitializer& ObjectInitializer);
	//set location relative to the location of (parent) tetromino
	virtual void SetLocation(FVector tetrominoLocation);
	//set location difference (from parent tetromino location)
	virtual void SetDifference(int column, int row);
	//get column difference
	virtual int GetColumnDifference();
	virtual int GetRowDifference();
	virtual void MoveDown();
	virtual void SetColor(int i);

	UPROPERTY(EditAnywhere, Category = "Mesh") 
		UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, Category = "Material")
		UMaterialInstanceDynamic* DynamicMaterialInst;

private:
	int columnDifference;
	int rowDifference;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
