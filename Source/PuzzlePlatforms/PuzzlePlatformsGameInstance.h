#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "MenuSystem/MenuInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"

class UMainMenu;
class UInGameMenu;
class FOnlineSessionSearch;

UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer &ObjectInitializer);

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	UFUNCTION(Exec)
	virtual void Host() override;

	UFUNCTION(Exec)
	virtual void Join(uint32 Index) override;

	virtual void LoadMainMenuMap() override;

	virtual void RefreshServerList() override;

private:
	TSubclassOf<UUserWidget> MainMenuClass;
	TSubclassOf<UUserWidget> InGameMenuClass;

	UMainMenu *Menu;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	void CreateSession();

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool Success);
};
