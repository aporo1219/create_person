// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GlobalData.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UGlobalData : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = "PlayerData")
		int32 Score[5];

	UGlobalData()
	{
		//InitData();
	}

	void InitData()
	{
		for (int i = 0; i < 5; i++)
			Score[i] = 0;
	}

	void SetData(int32 x);
	int32* GetData();
};
