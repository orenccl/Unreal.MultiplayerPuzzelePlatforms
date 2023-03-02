// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class UButton;
class IMenuInterface;

UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	void SetMenuInterface(IMenuInterface *Interface);

	void Setup();

	void Teardown();

private:
	UPROPERTY(meta = (BindWidget))
	UButton *Host;

	UPROPERTY(meta = (BindWidget))
	UButton *Join;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	IMenuInterface *MenuInterface;
};
