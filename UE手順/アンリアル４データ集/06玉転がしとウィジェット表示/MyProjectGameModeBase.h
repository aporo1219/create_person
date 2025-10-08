// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MyUserWidget.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyProjectGameModeBase.generated.h"

/**
 * 
 */

UCLASS()
class MYPROJECT_API AMyProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	
	
	UPROPERTY()
		UMyUserWidget* GameWidget;

	int32 ItemsCollected = 0;
	int32 ItemsInLevel = 0;

	virtual void BeginPlay() override;

	void UpdateItemText();

public :
	AMyProjectGameModeBase();


	void ItemCollected();

};
