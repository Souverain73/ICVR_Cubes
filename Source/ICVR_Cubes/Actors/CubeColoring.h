// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "CubeColoring.generated.h"

/**
 * Data about Actors in camera view
 * User for store and sorting actors before separate in pools
 */
USTRUCT()
struct FActorInCameraInfo
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* ActorRef;
	
	UPROPERTY()
	float DistanceToCamera;

	FActorInCameraInfo() = default;

	FActorInCameraInfo(AActor* ActorRef, float DistanceToCamera)
		: ActorRef(ActorRef),
		  DistanceToCamera(DistanceToCamera)
	{
	}

	// Used for default TArray.Sort
	friend bool operator<(const FActorInCameraInfo& Lhs, const FActorInCameraInfo& RHS)
	{
		return Lhs.DistanceToCamera < RHS.DistanceToCamera;
	}
};

/**
 * Info about actors pool properties
 */
USTRUCT()
struct FColoredActorsPool
{
	GENERATED_BODY()

	/**
	 * Max distance from camera
	 * -1 = infinity
	 */
	UPROPERTY(EditAnywhere)
	float MaxDistance;

	/**
	 * Max count of objects in pool
	 * -1 = infinity
	 */
	UPROPERTY(EditAnywhere)
	int32 PoolSize;

	/**
	 * Material which set to Actor if it included in this pool
	 */
	UPROPERTY(EditAnywhere)
	UMaterialInterface* ColorMaterial;
};

/**
 * Main Class working with cube colors
 * Use CameraComponent to detect cubes in camera view
 * Color cubes using PoolSettings
 */
UCLASS()
class ICVR_CUBES_API ACubeColoring : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * Any actor with CameraComponent
	 * Used to get view info
	 */
	UPROPERTY(EditAnywhere)
	AActor* CameraActor;

	/**
	 * Class of actors that must be colored.
	 * Used to select only needed actors
	 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorsToColorClass;

	/**
	 * Default material which set to objects not in camera view
	 */
	UPROPERTY(EditAnywhere)
	UMaterialInterface* DefaultMaterial;

	UPROPERTY()
	UCameraComponent* CameraComponent;

	/**
	 * Pools and colors settings
	 */
	UPROPERTY(EditAnywhere, Category="Pool settings")
	TArray<FColoredActorsPool> PoolSettings;

	ACubeColoring();

protected:
	/**
	 *	Called when the game starts or when spawned
	 *	Check component setup correctness
	 */
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	/**
	 * Get all actors in camera view
	 * Iterate over all Actors of selected class and intersect with camera frustum
	 */
	void GetActorsInCameraView(TArray<FActorInCameraInfo>& OutArray) const;
	
	float GetDistanceFromCamera(const AActor* Actor) const;
	
	/**
	 * Changes Actor color using different materials
	 */
	void SetActorMaterial(AActor* Actor, UMaterialInterface* Material) const;
};
