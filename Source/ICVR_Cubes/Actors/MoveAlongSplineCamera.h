// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "MoveAlongSplineCamera.generated.h"

class UMoveAlongSplineMovementComponent;

/**
* Combination of Camera, Spline track and UMoveAlongSplineMovementComponent
*/
UCLASS()
class ICVR_CUBES_API AMoveAlongSplineCamera : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Movement")
	UMoveAlongSplineMovementComponent* MovementComponent;

	/**
	 * This object is moved by MovementComponent
	 */
	UPROPERTY(EditAnywhere, Category="Camera")
	USceneComponent* CameraMount;
	
	UPROPERTY(EditAnywhere, Category="Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, Category="Track")
	USplineComponent* SplineComponent;

	AMoveAlongSplineCamera(const FObjectInitializer& ObjectInitializer);
};
