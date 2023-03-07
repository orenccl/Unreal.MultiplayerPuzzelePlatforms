// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MenuInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Use this interface to injecting dependencies
 * Implement this interface if you want to use Menu System
 */
class PUZZLEPLATFORMS_API IMenuInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** Host a server */
	virtual void Host(FString ServerName) = 0;
	/** Join a server */
	virtual void Join(uint32 Index) = 0;

	virtual void LoadMainMenuMap() = 0;

	virtual void RefreshServerList() = 0;
};
