#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
    PrimaryActorTick.bCanEverTick = true;
    // Moving platform should always be Movable
    SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
    Super::BeginPlay();

    // If not server
    if (HasAuthority() == false)
    {
        return;
    }

    // Init value
    GlobalStartLocation = GetActorLocation();
    GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);

    // Set replicate setting
    SetReplicates(true);
    SetReplicateMovement(true);
}

void AMovingPlatform::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // If not server
    if (HasAuthority() == false)
    {
        return;
    }

    // If has no ActiveTrigger
    if (ActiveTriggers == 0)
    {
        return;
    }

    FVector CurrentLocation = GetActorLocation();

    // Swap start and target if reached target
    float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
    float JourneyTravelled = (CurrentLocation - GlobalStartLocation).Size();
    if (JourneyTravelled >= JourneyLength)
    {
        Swap<FVector>(GlobalStartLocation, GlobalTargetLocation);
    }

    // Calculate direction
    FVector JourneyDirection = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
    // Set new location
    SetActorLocation(CurrentLocation + JourneyDirection * Speed * DeltaTime);
}

void AMovingPlatform::AddActiveTrigger()
{
    ActiveTriggers++;
}

void AMovingPlatform::RemoveActiveTrigger()
{
    // Prevent negative number
    if (ActiveTriggers == 0)
    {
        return;
    }

    ActiveTriggers--;
}