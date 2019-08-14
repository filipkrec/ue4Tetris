// Fill out your copyright notice in the Description page of Project Settings.


#include "Block.h" 
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ABlock::ABlock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();
}

ABlock::ABlock(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));

	if (BaseMeshAsset.Object)
	{
		BaseMesh->SetStaticMesh(BaseMeshAsset.Object);
		BaseMesh->SetWorldScale3D(FVector(0.19, 0.19, 0.19));
		SetRootComponent(BaseMesh);
	}

}

void ABlock::SetLocation(FVector tetrominoLocation)
{
	FVector location = tetrominoLocation;
	location.X += (columnDifference * 20);
	location.Z -= (rowDifference * 20);
	SetActorLocation(location);
}

void ABlock::MoveDown()
{
	FVector location = GetActorLocation();
	location.Z -= 20;
	SetActorLocation(location);
}

void ABlock::SetDifference(int column, int row)
{
	columnDifference = column;
	rowDifference = row;
}

int ABlock::GetColumnDifference()
{
	return columnDifference;
}

int ABlock::GetRowDifference()
{
	return rowDifference;
}

// Called every frame
void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

