// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

const static FName SESSION_NAME = TEXT("Session Game");

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer &ObjectInitializer)
{
    // Find main menu widget BP classes
    static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
    if (!ensure(MainMenuBPClass.Class != nullptr))
    {
        return;
    }
    MainMenuClass = MainMenuBPClass.Class;

    // Find in game widget BP classes
    static ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
    if (!ensure(InGameMenuBPClass.Class != nullptr))
    {
        return;
    }
    InGameMenuClass = InGameMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
    Super::Init();

    // Try get OnlineSubsystem
    IOnlineSubsystem *OnlineSubsystem = IOnlineSubsystem::Get();
    if (!ensure(OnlineSubsystem != nullptr))
    {
        UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem not found!"));
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem found: %s"), *OnlineSubsystem->GetSubsystemName().ToString());

    // Try get SessionInterface
    SessionInterface = OnlineSubsystem->GetSessionInterface();
    if (!SessionInterface.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("SessionInterface not found!"));
        return;
    };

    // Bind session delegates
    SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
    SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
    SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);
}

void UPuzzlePlatformsGameInstance::RefreshServerList()
{
    if (!SessionInterface.IsValid())
    {
        return;
    };

    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    if (!SessionSearch.IsValid())
    {
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Starting find session"));
    SessionSearch->bIsLanQuery = true;
    SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UPuzzlePlatformsGameInstance::LoadMainMenuMap()
{
    APlayerController *PlayerController = GetFirstLocalPlayerController();
    if (!ensure(PlayerController != nullptr))
        return;

    // Use client travel command to leave current server.
    PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
    // Try create
    if (!ensure(MainMenuClass != nullptr))
        return;

    Menu = CreateWidget<UMainMenu>(this, MainMenuClass);
    if (!ensure(Menu != nullptr))
        return;

    // Init
    Menu->Setup();
    Menu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::LoadInGameMenu()
{
    // Try create
    if (!ensure(InGameMenuClass != nullptr))
        return;

    UMenuWidget *InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
    if (!ensure(InGameMenu != nullptr))
        return;

    // Init
    InGameMenu->Setup();
    InGameMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Host()
{
    if (!SessionInterface.IsValid())
    {
        return;
    };

    // If already exist then destroy
    FNamedOnlineSession *ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
    if (ExistingSession)
    {
        SessionInterface->DestroySession(SESSION_NAME);
    }
    else
    {

        CreateSession();
    }
}

void UPuzzlePlatformsGameInstance::Join(const FString &Address)
{
    if (!SessionInterface.IsValid())
    {
        return;
    };

    UEngine *Engine = GetEngine();
    if (!ensure(Engine != nullptr))
        return;

    Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Join Server..."));

    APlayerController *PlayerController = GetFirstLocalPlayerController();
    if (!ensure(PlayerController != nullptr))
        return;

    // Join server by IP address
    PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
    if (!SessionInterface.IsValid())
    {
        return;
    };

    FOnlineSessionSettings SessionSettings;
    SessionSettings.bIsLANMatch = true;
    SessionSettings.NumPublicConnections = 2;
    SessionSettings.bShouldAdvertise = true;

    SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
    if (!Success)
    {
        UE_LOG(LogTemp, Warning, TEXT("Create Session Fail!"));
        return;
    }

    UEngine *Engine = GetEngine();
    if (!ensure(Engine != nullptr))
        return;

    Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting..."));

    UWorld *World = GetWorld();
    if (!ensure(World != nullptr))
        return;

    // If game is already server, also bring all child to new server together.
    World->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
}

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool Success)
{
    UE_LOG(LogTemp, Warning, TEXT("Finish find session %i"), Success);

    if (!SessionSearch.IsValid())
        return;

    if (!ensure(Menu != nullptr))
        return;

    TArray<FString> ServerNames;
    TArray<FOnlineSessionSearchResult> Results = SessionSearch->SearchResults;
    for (const FOnlineSessionSearchResult &Result : Results)
    {
        ServerNames.Add(Result.GetSessionIdStr());
    }

    Menu->SetServerList(ServerNames);
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
    if (!Success)
    {
        return;
    }

    // Recreate session after successfully destroy it
    CreateSession();
}
