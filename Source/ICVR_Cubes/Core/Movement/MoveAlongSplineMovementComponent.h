// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SplineComponent.h"
#include "GameFramework/MovementComponent.h"
#include "MoveAlongSplineMovementComponent.generated.h"

/**
 * Movement component for move object along the spline
 */
UCLASS()
class ICVR_CUBES_API UMoveAlongSplineMovementComponent : public UMovementComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category="Movement")
	USplineComponent* SplineRef;

	UPROPERTY(EditAnywhere, Category="Movement")
	float DistancePerSecond = 100.f;

	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	float SplineLength = 0.f;
	float CurrentDistance = 0.f;
};
