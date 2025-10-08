// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAttackTask.h"
#include "AIController.h"
#include "EnemyActor.h"
#include "MyPawn.h"
#include "Kismet/KismetSystemLibrary.h"

UMyAttackTask::UMyAttackTask(FObjectInitializer const& ObjectInitializer):UBTTask_BlackboardBase{ ObjectInitializer }
{
	// BehaviorTreeのタスク欄に表示する名前
	NodeName = TEXT("MyAttackTask");
}


void UMyAttackTask::OnGameplayTaskActivated(UGameplayTask& Task)
{

}

//行動タスクの内容
EBTNodeResult::Type UMyAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//AIコントローラ獲得
	AEnemyAIController* aic = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (aic != nullptr)
	{
		//AIコントローラーが持っているポーンを獲得
		AEnemyActor* enemy = Cast<AEnemyActor>(aic->GetPawn());
		if (enemy != nullptr)
		{
			//ブラックボード獲得
			auto* BB_Component = OwnerComp.GetBlackboardComponent();
			if (BB_Component != nullptr)
			{
				//ブラックボードが持つ変数から、設定したプレイヤーの情報獲得
				AMyPawn* player = Cast<AMyPawn>(BB_Component->GetValueAsObject("ObjectKey"));

				if (player != nullptr)
				{
					FVector enemy_pos = enemy->mCapsule->GetRelativeLocation();
					FVector play_pos = player->mCapsule->GetRelativeLocation();
					FVector w = enemy_pos - play_pos;

					//距離に応じて、行動させている
					float r = sqrt( powf( w.Size(), 2));
					if (r < 200)
					{
						//成功　ジャンプ攻撃
						enemy->mCapsule->AddForce(FVector(0,0,1) * 400000);
						return EBTNodeResult::Succeeded;
					}
					else
					{
						//失敗
						return EBTNodeResult::Failed;
					}
				}
			}
		}
	}

	
	return EBTNodeResult::Failed;
}