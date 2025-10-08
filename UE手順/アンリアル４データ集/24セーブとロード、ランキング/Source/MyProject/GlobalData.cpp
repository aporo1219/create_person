// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalData.h"
#include "Kismet/KismetSystemLibrary.h"//PrintString

void UGlobalData::SetData(int32 x)
{
	Score[4] = x;

	for (int i = 0; i < 4; i++)
	{
		for (int j = i; j < 5; j++)
		{
			if (Score[i] < Score[j])
			{
				int32 w= Score[i];
				Score[i] = Score[j];
				Score[j] = w;
			}
		}
	}

	for(int i = 0; i < 5; i++)
	{
		UKismetSystemLibrary::PrintString(this,
			FString::Printf(TEXT("%d"),Score[i]),
			true, true, FColor::Cyan, 2.f);
	}
}

int32* UGlobalData::GetData()
{
	return Score;
}

