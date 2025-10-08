// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Pawn.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	//���s���x 
	UPROPERTY(BlueprintReadWrite) //����UPROPERTY���L�ڂ��邱�Ƃ�ABP����������p�ł��܂��B 
		float Speed = 0.0f;

	//�����X�C�b�`
	UPROPERTY(BlueprintReadWrite)
		bool IsWalk;
public:
	//ABP��Tick�Ɠ��l�ɖ��t���[���Ăяo����鏈���̃I�[�o�[���C�h�ł��B 
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
