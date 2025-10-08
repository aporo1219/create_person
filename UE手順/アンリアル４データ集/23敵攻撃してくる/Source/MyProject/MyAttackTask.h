// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MyAttackTask.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyAttackTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	explicit UMyAttackTask(FObjectInitializer const& ObjectInitializer);
	// �^�X�N���s���e
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
	

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

private:
	// �u���b�N�{�[�h�ɕۊǂ���p�̃L�[���
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
		FBlackboardKeySelector Attack_on;
};
	
