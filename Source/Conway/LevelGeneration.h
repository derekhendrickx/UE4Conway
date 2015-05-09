// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LevelGeneration.generated.h"

UCLASS()
class CONWAY_API ALevelGeneration : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelGeneration();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void PostInitProperties() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Generation)
	int32 Width = 32;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Generation)
	int32 Height = 32;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Generation)
	int32 TileSize = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Generation)
	float InitialChance = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Generation)
	int32 SimulationStep = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Generation)
	FRandomStream RandStream;

	UInstancedStaticMeshComponent* Tile;

	UInstancedStaticMeshComponent* Cube;

	TArray<int32> Level;
	
private:
	void Conway();

	void Init();

	void Simulation(TArray<int32>& NewTiles);

	void FillLevel();

	void ClearLevel();

	int32 CountAliveNeighbors(const int32 X, const int32 Y);

	void OneDimensionIndexToTwoDimension(const int32 Index, int32& X, int32& Y);

	int32 CheckPosition(const int32 X, const int32 Y);
};
