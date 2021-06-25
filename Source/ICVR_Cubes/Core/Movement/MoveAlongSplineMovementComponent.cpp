// Fill out your copyright notice in the Description page of Project Settings.

#include "ICVR_Cubes/Core/Movement/MoveAlongSplineMovementComponent.h"

void UMoveAlongSplineMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	SplineLength = SplineRef->GetSplineLength();

	checkf(IsValid(SplineRef), TEXT("UMoveAlongSplineMovementComponent must have valid SplineRef"))
	checkf(IsValid(UpdatedComponent), TEXT("UMoveAlongSplineMovementComponent must have valid UpdatedComponent"))
	
	UpdatedComponent->SetWorldRotation(SplineRef->GetRotationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World));
}

void UMoveAlongSplineMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	if (!IsValid(SplineRef) || !IsValid(UpdatedComponent))
	{
		return;
	}

	const FVector OldLocation = UpdatedComponent->GetComponentLocation();

	const float NewDistance = FMath::Fmod(CurrentDistance + DistancePerSecond * DeltaTime, SplineLength);

	const FVector NewLocation = SplineRef->GetLocationAtDistanceAlongSpline(NewDistance, ESplineCoordinateSpace::World);
	const FRotator NewRotation = SplineRef->GetRotationAtDistanceAlongSpline(NewDistance, ESplineCoordinateSpace::World);
	
	FHitResult SweepHitResult;
	const FVector Delta = NewLocation - OldLocation;
	SafeMoveUpdatedComponent(Delta, NewRotation, false, SweepHitResult);

	CurrentDistance = NewDistance;
}