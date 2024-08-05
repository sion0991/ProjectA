// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridSpawner.generated.h"

UCLASS()
class PROJECTA_API AGridSpawner : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int NumRows;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int NumColumns;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float TileSize;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float LineThickness;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FLinearColor LineColor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FLinearColor SelectionColor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float LineOpacity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float SelectionOpacity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMaterial* DynamicMat;

private:
	void CreateLine(FVector Start, FVector End, float Thickness, TArray<FVector>& Vertices, TArray<int>& Triangles);
	float GridWidth();
	float GridHeight();
	UMaterialInstanceDynamic* CreateMaterialInstance(FLinearColor Color, float Opacity);
	UFUNCTION(BlueprintCallable)
	void LocationToTile(FVector Location, int& Row, int& Column, bool& IsValid);
	UFUNCTION(BlueprintCallable)
	void TileToGridLocation(int Row, int Column, bool IsCenter, FVector2D& GridLocation, bool& IsValid);
	UFUNCTION(BlueprintCallable)
	void SetSelectTile(int Row, int Column);
	bool TileValid(int Row, int Column);
	UFUNCTION(BlueprintCallable)
		FVector2D GetSelectPosition();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UProceduralMeshComponent* LineProceduralMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UProceduralMeshComponent* SelectionProceduralMesh;
public:
	AGridSpawner();

	virtual void OnConstruction(const FTransform& Transform) override;
};
