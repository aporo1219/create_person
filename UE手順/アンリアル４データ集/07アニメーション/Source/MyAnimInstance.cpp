// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "MyPawn.h"

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    //Superの呼び出し
    Super::NativeUpdateAnimation(DeltaSeconds);

    Speed  = 0.0f;
    IsWalk = false;

    //ABPのオーナーを取得する
    AMyPawn* p = Cast<AMyPawn>(GetOwningActor());

    //pにまつわる処理
    if (p != nullptr)
    {
        Speed = p->Speed;
        IsWalk = p->IsWalk;
        
        
    }
}