// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"
#include "Components/TextBlock.h"

void UMyUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    // 継承元処理
    Super::NativeTick(MyGeometry, InDeltaTime);

    // テキストブロックの取得
    UTextBlock* pTextBlock = Cast<UTextBlock>(GetWidgetFromName("TextBlock_0"));
    // テキスト書き換え
    if (pTextBlock) {
        // テキスト変更
        FString _Str = FString::Printf(TEXT("Count: %d / %d"), mItemsCollected, mItemINLevel);
        pTextBlock->SetText( FText::FromString(_Str));

        // カラー設定
        //pTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 0.f, 0.f, 1.f)));
    }

}