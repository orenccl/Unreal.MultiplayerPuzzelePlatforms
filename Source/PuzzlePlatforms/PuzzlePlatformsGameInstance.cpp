// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer &ObjectInitializer)
{
    static ConstructorHelpers::FClassFinder<UMainMenu> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
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
        return;

    MainMenu = CreateWidget<UMainMenu>(this, MainMenuClass);
    if (!ensure(MainMenu != nullptr))
        return;

    MainMenu->Setup();
    MainMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Host()
{
    if (MainMenu != nullptr)
    {
        MainMenu->Teardown();
    }

    UEngine *Engine = GetEngine();
    if (!ensure(Engine != nullptr))
        return;

    UWorld *World = GetWorld();
    if (!ensure(World != nullptr))
        return;

    World->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
}

void UPuzzlePlatformsGameInstance::Join(const FString &Address)
{
    UEngine *Engine = GetEngine();
    if (!ensure(Engine != nullptr))
        return;

    APlayerController *PlayerController = GetFirstLocalPlayerController();
    if (!ensure(PlayerController != nullptr))
        return;

    PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}