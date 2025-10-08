// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

void UMyUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    // 継承元処理
    Super::NativeTick(MyGeometry, InDeltaTime);
    FString Str;
    //テキスト変更
    // TIMEテキストブロック
    UTextBlock* pTextBlock = Cast<UTextBlock>(GetWidgetFromName("TextBlock_0"));
    if (pTextBlock) 
    {
        Str = FString::Printf(TEXT("TIME :%d"), mTime);
        pTextBlock->SetText(FText::FromString(Str));
    }

    // POINTテキストブロック
    pTextBlock = Cast<UTextBlock>(GetWidgetFromName("TextBlock_1"));
    if (pTextBlock) 
    {
        Str = FString::Printf(TEXT("POINT :%d"), mPoint);
        pTextBlock->SetText(FText::FromString(Str));
    }

}