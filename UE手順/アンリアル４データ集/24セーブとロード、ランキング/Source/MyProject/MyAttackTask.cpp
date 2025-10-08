// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAttackTask.h"
#include "AIController.h"
#include "EnemyActor.h"
#include "MyPawn.h"
#include "Kismet/KismetSystemLibrary.h"

UMyAttackTask::UMyAttackTask(FObjectInitializer const& ObjectInitializer):UBTTask_BlackboardBase{ ObjectInitializer }
{
	// BehaviorTree�̃^�X�N���ɕ\�����閼�O
	NodeName = TEXT("MyAttackTask");
}


void UMyAttackTask::OnGameplayTaskActivated(UGameplayTask& Task)
{

}

//�s���^�X�N�̓��e
EBTNodeResult::Type UMyAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//AI�R���g���[���l��
	AEnemyAIController* aic = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (aic != nullptr)
	{
		//AI�R���g���[���[�������Ă���|�[�����l��
		AEnemyActor* enemy = Cast<AEnemyActor>(aic->GetPawn());
		if (enemy != nullptr)
		{
			//�u���b�N�{�[�h�l��
			auto* BB_Component = OwnerComp.GetBlackboardComponent();
			if (BB_Component != nullptr)
			{
				//�u���b�N�{�[�h�����ϐ�����A�ݒ肵���v���C���[�̏��l��
				AMyPawn* player = Cast<AMyPawn>(BB_Component->GetValueAsObject("ObjectKey"));

				if (player != nullptr)
				{
					FVector enemy_pos = enemy->mCapsule->GetRelativeLocation();
					FVector play_pos = player->mCapsule->GetRelativeLocation();
					FVector w = enemy_pos - play_pos;

					//�����ɉ����āA�s�������Ă���
					float r = sqrt( powf( w.Size(), 2));
					if (r < 200)
					{
						//�����@�W�����v�U��
						enemy->mCapsule->AddForce(FVector(0,0,1) * 400000);
						return EBTNodeResult::Succeeded;
					}
					else
					{
						//���s
						return EBTNodeResult::Failed;
					}
				}
			}
		}
	}

	
	return EBTNodeResult::Failed;
}