// Copyright Epic Games, Inc. All Rights Reserved.

#include "BatteryCollectorGameMode.h"
#include "BatteryCollectorCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PawnMovementComponent.h"

#include "SpawnVolume.h"

#include "Blueprint/UserWidget.h"

ABatteryCollectorGameMode::ABatteryCollectorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// Tick in the game mode disabled so it should set to be true
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	// base decay rate
	DecayRate = 0.01f;
}

void ABatteryCollectorGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check that we are using the battery collector character
	ABatteryCollectorCharacter* MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	if(MyCharacter)
	{
		// If our power is greater than needed to win, set the game's state to won
		if(MyCharacter->GetCurrentPower() > PowerToWin )
		{
			SetCurrentState(EBatterPlayState::EWon);
		}
		// if the character's power is positive
		else if (MyCharacter->GetCurrentPower() > 0)
		{
			// decrease the character's power using the decay rate
			MyCharacter->UpdatePower(-DeltaTime * DecayRate * (MyCharacter->GetInitialPower()));
		}
		else
		{
			SetCurrentState(EBatterPlayState::EGameOver);
		}
	}
}

float ABatteryCollectorGameMode::GetPowerToWin() const
{
	return PowerToWin;
}

void ABatteryCollectorGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Find all spawn volume Actors 
	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);

	for (auto Actor : FoundActors)
	{
		ASpawnVolume* SpawnVolumeActor = Cast<ASpawnVolume>(Actor);

		if (SpawnVolumeActor)
		{
			SpawnVolumeActors.AddUnique(SpawnVolumeActor);
		}
	}

	SetCurrentState(EBatterPlayState::EPlaying);

	// set the score to beat
	ABatteryCollectorCharacter* MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	if(MyCharacter)
	{
		PowerToWin = (MyCharacter->GetInitialPower() * 1.25f);
	}

	if(HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

		if(CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

EBatterPlayState ABatteryCollectorGameMode::GetCurrentState() const
{
	return CurrentState;
}

void ABatteryCollectorGameMode::SetCurrentState(EBatterPlayState NewState)
{	
	// set current state 
	CurrentState = NewState;

	// handle the new current state
	HandleNewState(NewState);
}

void ABatteryCollectorGameMode::HandleNewState(EBatterPlayState NewState)
{
	switch (NewState)
	{
		// If the game is playing
		case EBatterPlayState::EPlaying:
		{
			// spawn volumes active
			for( ASpawnVolume* Volume : SpawnVolumeActors)
			{
				Volume->SetSpawningActive(true);
			}
		}
		break;
		// If we've lost the game
		case EBatterPlayState::EGameOver:
		{
			// spawn volumes inactive
			for (ASpawnVolume* Volume : SpawnVolumeActors)
			{
				Volume->SetSpawningActive(false);
			}

			// block player input
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
			if(PlayerController)
			{
				PlayerController->SetCinematicMode(true, false, false, true, true);
			}
			
			// ragdoll the character
			ACharacter* MyCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			if(MyCharacter)
			{
				MyCharacter->GetMesh()->SetSimulatePhysics(true);

				MyCharacter->GetMovementComponent()->MovementState.bCanJump = false;
			}

		}
		break;
		// If we've won the game
		case EBatterPlayState::EWon:
		{
			// spawn volumes inactive
			for (ASpawnVolume* Volume : SpawnVolumeActors)
			{
				Volume->SetSpawningActive(false);
			}
		}
		break;
		// Unknown / default state
		case EBatterPlayState::EUnknown:
		default:
		{
			// do nothing
		}
		break;
	}
}
