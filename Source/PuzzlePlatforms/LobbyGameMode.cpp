// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "TimerManager.h"
#include "PuzzlePlatformsGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController *NewPlayer)
{
    Super::PostLogin(NewPlayer);
    ++NumberOfPlayers;

    if (NumberOfPlayers < 2)
        return;

    // Prevent timer trigger multiple times
    if (GameStartTimer.IsValid())
        return;

    GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameMode::StartGame, 10);
}

void ALobbyGameMode::Logout(AController *Exiting)
{
    Super::Logout(Exiting);
    --NumberOfPlayers;
}

void ALobbyGameMode::StartGame()
{
    // Prevent find/join session after start
    UPuzzlePlatformsGameInstance *GameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());
    if (!ensure(GameInstance != nullptr))
        return;

    GameInstance->StartSession();

    UWorld *World = GetWorld();
    if (!ensure(World != nullptr))
        return;

    // Travel to game map with transition map
    bUseSeamlessTravel = true;
    World->ServerTravel("/Game/PuzzlePlatforms/Maps/Game?listen");
}
