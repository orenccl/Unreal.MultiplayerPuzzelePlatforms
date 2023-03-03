// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformTrigger.h"
#include "Components/BoxComponent.h"
#include "MovingPlatform.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create overlap component
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Volume"));
	if (!ensure(TriggerVolume != nullptr))
		return;
	// Replace root
	RootComponent = TriggerVolume;

	// Create visual component
	PressurePad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pressure Pad"));
	if (!ensure(PressurePad != nullptr))
		return;
	PressurePad->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();

	if (!ensure(TriggerVolume != nullptr))
		return;

	// Bind overlap event
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnBeginOverlap);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnEndOverlap);
}

void APlatformTrigger::OnBeginOverlap(UPrimitiveComponent *OverlappedComp, AActor *Other, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// Active all MovingPlatform in list
	for (AMovingPlatform *Platform : PlatformsToTrigger)
	{
		Platform->AddActiveTrigger();
	}
}

void APlatformTrigger::OnEndOverlap(UPrimitiveComponent *OverlappedComp, AActor *Other, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	// Deactive all MovingPlatform in list
	for (AMovingPlatform *Platform : PlatformsToTrigger)
	{
		Platform->RemoveActiveTrigger();
	}
}