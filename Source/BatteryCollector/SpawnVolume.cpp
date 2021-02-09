// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"

// All of the Components used in cpp file should be included by individually
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create the Box Component to represent spawn volume
	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));

	RootComponent = WhereToSpawn;

}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetRandonPointInVolume()
{
	FVector SpawnOrigin = WhereToSpawn->Bounds.Origin;
	FVector SpawnExtent = WhereToSpawn->Bounds.BoxExtent;

	// Get random point in range of box component
	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
}

