// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "EnemyActor.h"


AEnemyAIController::AEnemyAIController()
{
    mBehavior = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior"));
    mBlackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));

    // 作成したビヘイビアツリーを設定
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

    // AIControllerがPawn所持した際にBBとBTを使用
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
    //コードのエラーが発生する箇所に直接記述するブレイク
    ensure(mBlackboard);
    // ブラックボードで作成したObjectKeyというキーにプレイヤー情報を入れる
    mBlackboard->SetValueAsObject(PlayerKeyName, player);
}

AEnemyActor* AEnemyAIController::GetPlayerKey()
{
    ensure(mBlackboard);
    return Cast<AEnemyActor>(mBlackboard->GetValueAsObject(PlayerKeyName));
}

