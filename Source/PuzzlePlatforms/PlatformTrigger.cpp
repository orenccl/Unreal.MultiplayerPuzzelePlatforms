// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformTrigger.h"
#include "Components/BoxComponent.h"
#include "MovingPlatform.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Init basic component
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Volume"));
	if (!ensure(TriggerVolume != nullptr))
		return;

	RootComponent = TriggerVolume;

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

	// Init overlap event
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnBeginOverlap);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnEndOverlap);
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlatformTrigger::OnBeginOverlap(UPrimitiveComponent *OverlappedComp, AActor *Other, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	for (AMovingPlatform *Platform : PlatformsToTrigger)
	{
		Platform->AddActiveTrigger();
	}
}

void APlatformTrigger::OnEndOverlap(UPrimitiveComponent *OverlappedComp, AActor *Other, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	for (AMovingPlatform *Platform : PlatformsToTrigger)
	{
		Platform->RemoveActiveTrigger();
	}
}