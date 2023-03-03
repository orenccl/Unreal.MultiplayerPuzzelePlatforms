// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include "Components/Button.h"
#include "MenuInterface.h"

bool UInGameMenu::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success)
    {
        return false;
    }

    // Setup
    if (ensure(CancelButton != nullptr))
    {
        CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::CancelPressed);
    }

    if (ensure(QuitButton != nullptr))
    {
        QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitPressed);
    }

    return true;
}

void UInGameMenu::CancelPressed()
{
    NativeDestruct();
}

void UInGameMenu::QuitPressed()
{
    if (!ensure(MenuInterface != nullptr))
        return;

    MenuInterface->LoadMainMenuMap();
}
