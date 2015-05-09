// Fill out your copyright notice in the Description page of Project Settings.

#include "Conway.h"
#include "LevelGeneration.h"


// Sets default values
ALevelGeneration::ALevelGeneration()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tile = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName(TEXT("Tile")));
	Tile->SetMobility(EComponentMobility::Static);
	Tile->AttachTo(RootComponent);

	Cube = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName(TEXT("Cube")));
	Cube->SetMobility(EComponentMobility::Static);
	Cube->AttachTo(Tile);

	RandStream = FRandomStream();
}

void ALevelGeneration::OnConstruction(const FTransform& Transform)
{
	Conway();
}

void ALevelGeneration::PostInitProperties()
{
	Super::PostInitProperties();

	//Conway();
}

#if WITH_EDITOR
void ALevelGeneration::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	//Conway();

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

// Called when the game starts or when spawned
void ALevelGeneration::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelGeneration::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ALevelGeneration::Conway()
{
	Init();

	for (int32 i = 0; i < SimulationStep; i++)
	{
		TArray<int32> NewLevel;
		NewLevel.SetNum(Level.Num());

		Simulation(NewLevel);
		
		Level = NewLevel;
	}

	ClearLevel();

	FillLevel();
}

void ALevelGeneration::Init()
{
	Level.Empty();

	UE_LOG(LogTemp, Warning, TEXT("Before init level size = %d"), Level.Num());

	int32 size = Width * Height;

	for (int32 i = 0; i < size; i++)
	{
		if (RandStream.FRand() < InitialChance)
		{
			Level.Add(1);
		}
		else
		{
			Level.Add(0);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("After init level size = %d"), Level.Num());
}

void ALevelGeneration::Simulation(TArray<int32>& NewLevel)
{
	UE_LOG(LogTemp, Warning, TEXT("Old level size = %d"), Level.Num());

	UE_LOG(LogTemp, Warning, TEXT("New level size = %d"), NewLevel.Num());

	for (int32 i = 0; i < Level.Num(); i++)
	{
		int32 x = 0, y = 0;
		OneDimensionIndexToTwoDimension(i, x, y);

		int32 nbAlive = CountAliveNeighbors(x, y);
		if (Level[i])
		{
			if (nbAlive < 3)
			{
				NewLevel[i] = 0;
			}
			else
			{
				NewLevel[i] = 1;
			}
		}
		else
		{
			if (nbAlive > 4)
			{
				NewLevel[i] = 1;
			}
			else
			{
				NewLevel[i] = 0;
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Final new level size = %d"), NewLevel.Num());
}

void ALevelGeneration::FillLevel()
{
	for (int32 i = 0; i < Level.Num(); i++)
	{
		int32 x = i % Height * TileSize;
		int32 y = i / Width * TileSize;

		FVector position(x, y, 0.0f);

		if (Level[i])
		{
			Cube->AddInstance(FTransform(position));
		}
		else
		{
			Tile->AddInstance(FTransform(position));
		}
	}
}

void ALevelGeneration::ClearLevel()
{
	Cube->ClearInstances();
	Tile->ClearInstances();
}

int32 ALevelGeneration::CountAliveNeighbors(const int32 X, const int32 Y)
{
	int32 count = 0;

	int32 Moore[] = { -1, -1, 0, -1, 1, -1, -1, 0, 1, 0, -1, 1, 0, 1, 1, 1 };

	for (int32 i = 0; i < ARRAY_COUNT(Moore); i += 2)
	{
		int32 position = CheckPosition(X + Moore[i], Y + Moore[i + 1]);

		if (position != -1)
		{
			if (Level[position])
			{
				count++;
			}
		}
		else
		{
			count++;
		}
	}

	return count;
}

void ALevelGeneration::OneDimensionIndexToTwoDimension(const int32 Index, int32& X, int32& Y)
{
	if (Index >= 0)
	{
		Y = Index / Width;
		X = Index - (Y * Height);
	}
}

int32 ALevelGeneration::CheckPosition(const int32 X, const int32 Y)
{
	if (X < 0 || X >= Width || Y < 0 || Y >= Height)
	{
		return -1;
	}

	return Y * Width + X;
}