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
public : 
	int mTime{0};
	int mPoint{0};
	void SetTextData(int time, int point)
	{
		mTime = time;
		mPoint = point;
	}

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
