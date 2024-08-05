// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSpawner.h"
#include "ProceduralMeshComponent.h"

// Sets default values
AGridSpawner::AGridSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	NumRows = 10;
	NumColumns = 10;
	TileSize = 200;
	LineThickness = 10;
	LineColor = FLinearColor(0.04, 1, 1, 1);
	SelectionColor = FLinearColor(1, 1, 1, 1);
	LineOpacity = 1;
	SelectionOpacity = 1;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	LineProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("LineProceduralMesh"));
	LineProceduralMesh->SetupAttachment(RootComponent);
	SelectionProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("SelectionProceduralMesh"));
	SelectionProceduralMesh->SetupAttachment(RootComponent);
}

void AGridSpawner::CreateLine(FVector Start, FVector End, float Thickness, TArray<FVector>& Vertices, TArray<int>& Triangles)
{
	float HalfThickness = Thickness / 2.0f;
	FVector ThicknessDirection = (End - Start).GetSafeNormal(0.0001).Cross(FVector(0, 0, 1));

	TArray<int> MakeTriangles;
	MakeTriangles.Add(Vertices.Num() + 2);
	MakeTriangles.Add(Vertices.Num() + 1);
	MakeTriangles.Add(Vertices.Num() + 0);
	MakeTriangles.Add(Vertices.Num() + 2);
	MakeTriangles.Add(Vertices.Num() + 3);
	MakeTriangles.Add(Vertices.Num() + 1);
	
	Triangles.Append(MakeTriangles);

	TArray<FVector> MakeVertices;
	MakeVertices.Add(Start + (ThicknessDirection * HalfThickness));
	MakeVertices.Add(End + (ThicknessDirection * HalfThickness));
	MakeVertices.Add(Start - (ThicknessDirection * HalfThickness));
	MakeVertices.Add(End - (ThicknessDirection * HalfThickness));
	Vertices.Append(MakeVertices);

}

float AGridSpawner::GridWidth()
{
	return TileSize * NumColumns;
}

float AGridSpawner::GridHeight()
{
	return TileSize * NumRows;
}

UMaterialInstanceDynamic* AGridSpawner::CreateMaterialInstance(FLinearColor Color, float Opacity)
{
	UMaterialInstanceDynamic* GridMat = UMaterialInstanceDynamic::Create(DynamicMat, GetWorld());
	GridMat->SetVectorParameterValue("Color", Color);
	GridMat->SetScalarParameterValue("Opacity", Opacity);

	return GridMat;
}

void AGridSpawner::LocationToTile(FVector Location, int& Row, int& Column, bool& IsValid)
{
	Row = FMath::Floor((Location.X - GetActorLocation().X) / GridHeight() * NumRows);
	Column = FMath::Floor((Location.Y - GetActorLocation().Y) / GridWidth() * NumColumns);
	IsValid = TileValid(Row, Column);
}

void AGridSpawner::TileToGridLocation(int Row, int Column, bool IsCenter, FVector2D& GridLocation, bool& IsValid)
{
	float Half = IsCenter ? TileSize * 0.5f : 0;
	GridLocation = FVector2D(Row * TileSize + GetActorLocation().X + Half, Column * TileSize + GetActorLocation().Y + Half);

	IsValid = TileValid(Row, Column);
}

void AGridSpawner::SetSelectTile(int Row, int Column)
{
	FVector2D SelectLocation;
	bool Visible;
	TileToGridLocation(Row, Column, false, SelectLocation, Visible);
	SelectionProceduralMesh->SetVisibility(Visible);
	SelectionProceduralMesh->SetWorldLocation(FVector(SelectLocation.X, SelectLocation.Y, GetActorLocation().Z));

}

bool AGridSpawner::TileValid(int Row, int Column)
{
	return Row >= 0 && Row < NumRows && Column >= 0 && Column < NumColumns;
}

FVector2D AGridSpawner::GetSelectPosition()
{
	return FVector2D();
}

void AGridSpawner::OnConstruction(const FTransform& Transform)
{
	UMaterialInstanceDynamic* LineMaterialInstnace = CreateMaterialInstance(LineColor, LineOpacity);
	UMaterialInstanceDynamic* SelectMaterialInstance = CreateMaterialInstance(SelectionColor, SelectionOpacity);

	TArray<FVector> LineVertices;
	TArray<int> LineTriangles;
	for (int i = 0; i <= NumRows; i++)
	{
		float LineStart = i * TileSize;
		float LineEnd = GridWidth();
		
		CreateLine(FVector(LineStart, 0, 0), FVector(LineStart, LineEnd, 0), LineThickness, LineVertices, LineTriangles);
	}

	for (int i = 0; i <= NumColumns; i++)
	{
		float LineStart = i * TileSize;
		float LineEnd = GridHeight();
		
		CreateLine(FVector(0, LineStart, 0), FVector(LineEnd, LineStart, 0), LineThickness, LineVertices, LineTriangles);
	}
	TArray<FVector>Nor;
	TArray<FVector2D> UVs;
	TArray<FColor> VColors;
	TArray<FProcMeshTangent> Tangents;
	LineProceduralMesh->CreateMeshSection(0, LineVertices, LineTriangles,Nor,UVs,VColors,Tangents, false);
	LineProceduralMesh->SetMaterial(0, LineMaterialInstnace);

	TArray<FVector> SelectionVertices;
	TArray<int> SelcetionTriangles;
	float HalfSize = TileSize * 0.5f;
	CreateLine(FVector(0, HalfSize, 0), FVector(TileSize, HalfSize, 0), TileSize, SelectionVertices, SelcetionTriangles);
	SelectionProceduralMesh->SetVisibility(false);
	SelectionProceduralMesh->CreateMeshSection(0, SelectionVertices, SelcetionTriangles, Nor, UVs, VColors, Tangents, false);
	SelectionProceduralMesh->SetMaterial(0, SelectMaterialInstance);

}