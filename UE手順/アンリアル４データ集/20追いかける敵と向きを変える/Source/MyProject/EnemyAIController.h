// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	AEnemyAIController();
	
	UPROPERTY(Category = "AI", EditDefaultsOnly)
		UBehaviorTreeComponent* mBehavior;//�r�w�C�r�A�c���[�R���|�[�l���g

	UPROPERTY(Category = "AI", EditDefaultsOnly)
		UBlackboardComponent* mBlackboard;//�u���b�N�{�[�h�R���|�[�l���g

	UPROPERTY(Category = "AI", EditDefaultsOnly)
		UBehaviorTree* mBehaviorTree;//�쐬�����r�w�C�r�A�c���[�o�^�p

	UPROPERTY(Category = "AI", EditDefaultsOnly)
		FName PlayerKeyName;		//�v���C���[�L�[


	UFUNCTION()
		void SetPlayerKey(APawn* player);
	UFUNCTION()
		AEnemyActor* GetPlayerKey();


	// AIController��Pawn����
	virtual void OnPossess(class APawn* InPawn) override;

	// AIController��Pawn��������
	virtual void OnUnPossess() override;

	virtual void BeginPlay() override;

	
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return mBehavior; }
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return mBlackboard; }
	
};
