// Copyright Epic Games, Inc. All Rights Reserved.

#include "Kismet/KismetSystemLibrary.h"//PrintString
#include "Kismet/GameplayStatics.h"
#include "MyProjectGameModeBase.h"

void AMyProjectGameModeBase::BeginPlay()
{
	
}

void AMyProjectGameModeBase::ReStart()
{
	//レベル再オープン
	UWorld* World = GetWorld();
	if (World)
	{ 
		UGameplayStatics::OpenLevel(World, FName(*World->GetName()), false);
	}
}