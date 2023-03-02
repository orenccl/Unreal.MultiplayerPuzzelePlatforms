// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer &ObjectInitializer)
{
    static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
    if (!ensure(MainMenuBPClass.Class != nullptr))
    {
        return;
    }

    MainMenuClass = MainMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
    Super::Init();
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
    if (!ensure(MainMenuClass != nullptr))
    {
        return;
    }

    UUserWidget *MainMenu = CreateWidget<UUserWidget>(this, MainMenuClass);
    if (!ensure(MainMenu != nullptr))
    {
        return;
    }

    MainMenu->AddToViewport();

    APlayerController *PlayerController = GetFirstLocalPlayerController();
    if (!ensure(PlayerController != nullptr))
    {
        return;
    }

    // Focus widget and show mouse
    FInputModeUIOnly InputModeData;
    InputModeData.SetWidgetToFocus(MainMenu->TakeWidget());
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

    PlayerController->SetInputMode(InputModeData);
    PlayerController->SetShowMouseCursor(true);
}

void UPuzzlePlatformsGameInstance::Host()
{
    UEngine *Engine = GetEngine();
    if (!ensure(Engine != nullptr))
    {
        return;
    }

    UWorld *World = GetWorld();
    if (!ensure(World != nullptr))
    {
        return;
    }

    World->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
}

void UPuzzlePlatformsGameInstance::Join(const FString &Address)
{
    UEngine *Engine = GetEngine();
    if (!ensure(Engine != nullptr))
    {
        return;
    }

    APlayerController *PlayerController = GetFirstLocalPlayerController();
    if (!ensure(PlayerController != nullptr))
    {
        return;
    }

    PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}