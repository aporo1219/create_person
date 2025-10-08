// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MyUserWidget.h"
#include "GameFramework/GameModeBase.h"
#include "MyProjectGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMyProjectGameModeBase()
	{
		//TickÇåƒÇ—èoÇ∑Ç©Ç«Ç§Ç©ÉtÉâÉO
		PrimaryActorTick.bCanEverTick = true;
		mTime = 50000;
		mPoint = 0;
	}

	UPROPERTY()
		UMyUserWidget* GameWidget;

	int mTime{ 0 };
	int mPoint{ 0 };

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void ReStart();
	void RespawnPlayerStart(AController* Controller, AActor* PlayerStart);
};
