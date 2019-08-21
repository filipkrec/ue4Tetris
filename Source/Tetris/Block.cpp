// Fill out your copyright notice in the Description page of Project Settings.


#include "Block.h" 
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
class UMaterialInstanceDynamic;
// Sets default values
ABlock::ABlock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
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
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("Material'/Game/Colors/Blue.Blue'"));
	DynamicMaterialInst = UMaterialInstanceDynamic::Create(MaterialAsset.Object, nullptr);
	if (BaseMeshAsset.Object)
	{
		BaseMesh->SetStaticMesh(BaseMeshAsset.Object);
		BaseMesh->SetWorldScale3D(FVector(0.19, 0.19, 0.19));
		SetRootComponent(BaseMesh);
	}

}

void ABlock::SetColor(int i) 
{
	BlockColor color = BlockColor(i);
	FLinearColor materialColor;

	switch(color) {
	case red:
		materialColor.R = 1;
		materialColor.G = 0;
		materialColor.B = 0;
		break;
	case purple:
		materialColor.R = 0.5;
		materialColor.G = 0;
		materialColor.B = 1;
		break;
	case blue:
		materialColor.R = 0;
		materialColor.G = 1;
		materialColor.B = 1;
		break;
	case darkBlue:
		materialColor.R = 0;
		materialColor.G = 0;
		materialColor.B = 1;
		break;
	case orange:
		materialColor.R = 1;
		materialColor.G = 0.5;
		materialColor.B = 0;
		break;
	case yellow:
		materialColor.R = 1;
		materialColor.G = 1;
		materialColor.B = 0;
		break;
	case green:
		materialColor.R = 0;
		materialColor.G = 1;
		materialColor.B = 0;
		break;
	}

	materialColor.A = 1;

	if (DynamicMaterialInst)
	{
		DynamicMaterialInst->SetVectorParameterValue(FName("MaterialColor"), materialColor);
	}
	BaseMesh->SetMaterial(0, DynamicMaterialInst);
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

