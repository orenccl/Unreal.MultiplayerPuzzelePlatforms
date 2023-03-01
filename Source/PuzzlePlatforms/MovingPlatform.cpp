#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
    PrimaryActorTick.bCanEverTick = true;
    SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
    Super::BeginPlay();

    // Only work on server
    if (HasAuthority() == false)
    {
        return;
    }

    // Init value
    GlobalStartLocation = GetActorLocation();
    GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);

    // Init setting
    SetReplicates(true);
    SetReplicateMovement(true);
}

void AMovingPlatform::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Only work on server
    if (HasAuthority() == false)
    {
        return;
    }

    // Only moving when ActiveTriggers > 0
    if (ActiveTriggers == 0)
    {
        return;
    }

    FVector CurrentLocation = GetActorLocation();

    // Swap start and target if reached
    float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
    float JourneyTravelled = (CurrentLocation - GlobalStartLocation).Size();
    if (JourneyTravelled >= JourneyLength)
    {
        Swap<FVector>(GlobalStartLocation, GlobalTargetLocation);
    }

    // Calculate direction and set new location
    FVector JourneyDirection = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
    SetActorLocation(CurrentLocation + JourneyDirection * Speed * DeltaTime);
}

void AMovingPlatform::AddActiveTrigger()
{
    ActiveTriggers++;
}

void AMovingPlatform::RemoveActiveTrigger()
{
    // Prevent negative
    if (ActiveTriggers == 0)
    {
        return;
    }

    ActiveTriggers--;
}