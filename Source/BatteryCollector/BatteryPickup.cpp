// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryPickup.h"

// Set default values
ABatteryPickup::ABatteryPickup()
{
	GetMesh()->SetSimulatePhysics(true);

	// the base power level of the battery
	BatteryPower = 150.f;
}

float ABatteryPickup::GetPower()
{
	return BatteryPower;
}

void ABatteryPickup::WasCollected_Implementation()
{
	// Use the base pickup behavior
	Super::WasCollected_Implementation();

	// Destroy the battery
	Destroy();
}
