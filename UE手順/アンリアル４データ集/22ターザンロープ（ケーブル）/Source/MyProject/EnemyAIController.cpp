// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "EnemyActor.h"


AEnemyAIController::AEnemyAIController()
{
    mBehavior = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior"));
    mBlackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));

    // �쐬�����r�w�C�r�A�c���[��ݒ�
    ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Game/NewBehaviorTree.NewBehaviorTree"));
    mBehaviorTree = BTFinder.Object;

    PlayerKeyName = "ObjectKey";

}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();
}



void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // AIController��Pawn���������ۂ�BB��BT���g�p
    mBlackboard->InitializeBlackboard(*mBehaviorTree->BlackboardAsset);
    mBehavior->StartTree(*mBehaviorTree);
}

void AEnemyAIController::OnUnPossess()
{
    Super::OnUnPossess();
    mBehavior->StopTree();
}

void AEnemyAIController::SetPlayerKey(APawn* player)
{
    //�R�[�h�̃G���[����������ӏ��ɒ��ڋL�q����u���C�N
    ensure(mBlackboard);
    // �u���b�N�{�[�h�ō쐬����ObjectKey�Ƃ����L�[�Ƀv���C���[��������
    mBlackboard->SetValueAsObject(PlayerKeyName, player);
}

AEnemyActor* AEnemyAIController::GetPlayerKey()
{
    ensure(mBlackboard);
    return Cast<AEnemyActor>(mBlackboard->GetValueAsObject(PlayerKeyName));
}

