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
	//歩行速度 
	UPROPERTY(BlueprintReadWrite) //このUPROPERTYを記載することでABP側からも利用できます。 
		float Speed = 0.0f;

	//歩くスイッチ
	UPROPERTY(BlueprintReadWrite)
		bool IsWalk;
public:
	//ABPのTickと同様に毎フレーム呼び出される処理のオーバーライドです。 
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
