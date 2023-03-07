// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

class UTextBlock;
class UMainMenu;
class UButton;

/**
 * Child of ServerList
 */
UCLASS()
class PUZZLEPLATFORMS_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock *ServerName;

	UPROPERTY(meta = (BindWidget))
	UButton *RowButton;

	void Setup(UMainMenu *Parent, uint32 Index);

private:
	UFUNCTION()
	void OnClicked();

	UPROPERTY()
	UMainMenu *Parent;

	uint32 Index;
};
