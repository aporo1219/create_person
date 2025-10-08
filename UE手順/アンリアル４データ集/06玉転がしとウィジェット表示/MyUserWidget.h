// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetItemText(int32 ItemsCollected, int32 ItemINLevel)
	{
		mItemsCollected = ItemsCollected;
		mItemINLevel = ItemINLevel;

	}

	int32 mItemsCollected;
	int32 mItemINLevel;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
