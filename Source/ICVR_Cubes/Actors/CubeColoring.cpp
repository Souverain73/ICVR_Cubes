// Fill out your copyright notice in the Description page of Project Settings.


#include "ICVR_Cubes/Actors/CubeColoring.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

ACubeColoring::ACubeColoring()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACubeColoring::BeginPlay()
{
	Super::BeginPlay();
	checkf(IsValid(CameraActor), TEXT("No valid CameraActor"))
	
	CameraComponent = Cast<UCameraComponent>(CameraActor->GetComponentByClass(UCameraComponent::StaticClass()));
	checkf(IsValid(CameraComponent), TEXT("Camera actor have no valid CameraComponent"))

	checkf(IsValid(DefaultMaterial), TEXT("DefaultMaterial must be set"))
}

void ACubeColoring::Tick(float DeltaTime)
{
	TArray<FActorInCameraInfo> ActorsInCameraView;
	GetActorsInCameraView(ActorsInCameraView);
	ActorsInCameraView.Sort();

	int LastHandledActorIndex = 0;
	for (FColoredActorsPool Pool : PoolSettings)
	{
		int i;
		for (i = LastHandledActorIndex; i < ActorsInCameraView.Num(); i++)
		{
			if (Pool.MaxDistance != -1.f && ActorsInCameraView[i].DistanceToCamera > Pool.MaxDistance
				|| Pool.PoolSize != -1 && Pool.PoolSize <= i - LastHandledActorIndex)
			{
				break;
			}

			SetActorMaterial(ActorsInCameraView[i].ActorRef, Pool.ColorMaterial);
		}
		LastHandledActorIndex = i;
	}
}

void ACubeColoring::GetActorsInCameraView(TArray<FActorInCameraInfo>& OutArray) const
{
	FMinimalViewInfo CameraViewInfo;
	CameraComponent->GetCameraView(0, CameraViewInfo);
	
	FMatrix ViewMatrix;
	FMatrix ProjectionMatrix;
	FMatrix ViewProjectionMatrix;
	UGameplayStatics::GetViewProjectionMatrix(CameraViewInfo, ViewMatrix, ProjectionMatrix,ViewProjectionMatrix);

	FConvexVolume CameraFrustum;
	GetViewFrustumBounds(CameraFrustum, ViewProjectionMatrix, true);

	OutArray.Empty();
	for (TActorIterator<AActor> ActorIt(GetWorld(), ActorsToColorClass); ActorIt; ++ActorIt)
	{
		FVector BoundsOrigin;
		FVector BoundsExtent;
		ActorIt->GetActorBounds(false, BoundsOrigin, BoundsExtent);

		const bool IsActorInCameraFrustum = CameraFrustum.IntersectBox(BoundsOrigin, BoundsExtent);
		if (IsActorInCameraFrustum)
		{
			OutArray.Add(FActorInCameraInfo(*ActorIt, GetDistanceFromCamera(*ActorIt)));	
		}else
		{
			SetActorMaterial(*ActorIt, DefaultMaterial);
		}
	}
}

float ACubeColoring::GetDistanceFromCamera(const AActor* Actor) const
{
	return (CameraComponent->GetComponentLocation() - Actor->GetActorLocation()).Size();
}

void ACubeColoring::SetActorMaterial(AActor* Actor, UMaterialInterface* Material) const
{
	UStaticMeshComponent* ActorMesh = Cast<UStaticMeshComponent>(Actor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	ActorMesh->SetMaterial(0, Material);
}

