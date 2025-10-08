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
		UBehaviorTreeComponent* mBehavior;//ビヘイビアツリーコンポーネント

	UPROPERTY(Category = "AI", EditDefaultsOnly)
		UBlackboardComponent* mBlackboard;//ブラックボードコンポーネント

	UPROPERTY(Category = "AI", EditDefaultsOnly)
		UBehaviorTree* mBehaviorTree;//作成したビヘイビアツリー登録用

	UPROPERTY(Category = "AI", EditDefaultsOnly)
		FName PlayerKeyName;		//プレイヤーキー


	UFUNCTION()
		void SetPlayerKey(APawn* player);
	UFUNCTION()
		AEnemyActor* GetPlayerKey();


	// AIControllerのPawn所持
	virtual void OnPossess(class APawn* InPawn) override;

	// AIControllerのPawn所持解除
	virtual void OnUnPossess() override;

	virtual void BeginPlay() override;

	
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return mBehavior; }
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return mBlackboard; }
	
};
