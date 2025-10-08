// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

void UMyUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    // �p��������
    Super::NativeTick(MyGeometry, InDeltaTime);
    FString Str;
    //�e�L�X�g�ύX
    // TIME�e�L�X�g�u���b�N
    UTextBlock* pTextBlock = Cast<UTextBlock>(GetWidgetFromName("TextBlock_0"));
    if (pTextBlock) 
    {
        Str = FString::Printf(TEXT("TIME :%d"), mTime);
        pTextBlock->SetText(FText::FromString(Str));
    }

    // POINT�e�L�X�g�u���b�N
    pTextBlock = Cast<UTextBlock>(GetWidgetFromName("TextBlock_1"));
    if (pTextBlock) 
    {
        Str = FString::Printf(TEXT("POINT :%d"), mPoint);
        pTextBlock->SetText(FText::FromString(Str));
    }

}