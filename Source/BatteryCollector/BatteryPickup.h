// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "BatteryPickup.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYCOLLECTOR_API ABatteryPickup : public APickup
{
	GENERATED_BODY()

public:
	
	// Sets default values for this actor's properties
	ABatteryPickup();

	/** Override the WasColleted function - use Implementation because it's a Blueprint native event **/
	virtual void WasCollected_Implementation() override;

	/**	Public way to accerss the batter's power level **/
	float GetPower();

protected:
	
	/** Se the amount of power the battery gives to the character **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	float BatteryPower;

};
