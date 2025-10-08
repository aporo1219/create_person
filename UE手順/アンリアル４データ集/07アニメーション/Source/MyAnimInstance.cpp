// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "MyPawn.h"

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    //Super�̌Ăяo��
    Super::NativeUpdateAnimation(DeltaSeconds);

    Speed  = 0.0f;
    IsWalk = false;

    //ABP�̃I�[�i�[���擾����
    AMyPawn* p = Cast<AMyPawn>(GetOwningActor());

    //p�ɂ܂�鏈��
    if (p != nullptr)
    {
        Speed = p->Speed;
        IsWalk = p->IsWalk;
        
        
    }
}