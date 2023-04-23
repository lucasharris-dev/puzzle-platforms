// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

// the server always has authority, meaning anything that happens on the server is
   // accurate/right
AMovingPlatform::AMovingPlatform()
{
    PrimaryActorTick.bCanEverTick = true;

    SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        SetReplicates(true);
        SetReplicateMovement(true);
    }

    TargetLocation = FVector(0, 230, 330);
    GlobalStartLocation = GetActorLocation();
    GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation); // GetTransform() returns the position, rotation, and scale

}


void AMovingPlatform::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (HasAuthority()) // !HasAuthority() means on the client, not on the server
    {
        // FTransform CurrTransform = GetTransform();
        // UE_LOG(LogTemp, Warning, TEXT("%s"), *CurrTransform.ToString());

        FVector Location = GetActorLocation();
        float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
        float JourneyTraveled = (Location - GlobalStartLocation).Size();

        if (JourneyTraveled >= JourneyLength)
        {
            FVector TempSwap = GlobalStartLocation;
            GlobalStartLocation = GlobalTargetLocation;
            GlobalTargetLocation = TempSwap;
        }

        FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
        Location += DeltaTime * Speed * Direction;
        SetActorLocation(Location);
    }
}
