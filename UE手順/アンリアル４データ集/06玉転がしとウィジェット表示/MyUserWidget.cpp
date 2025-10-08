// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"
#include "Components/TextBlock.h"

void UMyUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    // �p��������
    Super::NativeTick(MyGeometry, InDeltaTime);

    // �e�L�X�g�u���b�N�̎擾
    UTextBlock* pTextBlock = Cast<UTextBlock>(GetWidgetFromName("TextBlock_0"));
    // �e�L�X�g��������
    if (pTextBlock) {
        // �e�L�X�g�ύX
        FString _Str = FString::Printf(TEXT("Count: %d / %d"), mItemsCollected, mItemINLevel);
        pTextBlock->SetText( FText::FromString(_Str));

        // �J���[�ݒ�
        //pTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 0.f, 0.f, 1.f)));
    }

}