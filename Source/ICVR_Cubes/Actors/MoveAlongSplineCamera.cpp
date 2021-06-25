// Fill out your copyright notice in the Description page of Project Settings.


#include "ICVR_Cubes/Actors/MoveAlongSplineCamera.h"

#include "ICVR_Cubes/Core/Movement/MoveAlongSplineMovementComponent.h"

AMoveAlongSplineCamera::AMoveAlongSplineCamera(const FObjectInitializer& ObjectInitializer): AActor(ObjectInitializer)
{
	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	RootComponent = SceneComponent;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("CameraTrack");
	SplineComponent->SetupAttachment(RootComponent);

	CameraMount = CreateDefaultSubobject<USceneComponent>("CameraMount");
	CameraMount->SetupAttachment(RootComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(CameraMount);
	CameraComponent->bCameraMeshHiddenInGame = false;
	
	MovementComponent = CreateDefaultSubobject<UMoveAlongSplineMovementComponent>("MovementComponent");
	MovementComponent->SetUpdatedComponent(CameraMount);
	MovementComponent->SplineRef = SplineComponent;
	
	PrimaryActorTick.bCanEverTick = true;
}

