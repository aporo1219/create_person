// Copyright Epic Games, Inc. All Rights Reserved.

#include "Kismet/KismetSystemLibrary.h"//PrintString
#include "Kismet/GameplayStatics.h"
#include "MyPawn.h"
#include "MyProjectGameModeBase.h"

void AMyProjectGameModeBase::BeginPlay()
{
	
}

//リスポーン　レベルオープン
void AMyProjectGameModeBase::ReStart()
{
	//レベル再オープン
	UWorld* World = GetWorld();
	if (World)
	{ 
		UGameplayStatics::OpenLevel(World, FName(*World->GetName()), false);
	}
}

//リスポーン 　プレイヤースタート位置からのリスタート
void AMyProjectGameModeBase::RespawnPlayerStart(AController* Controller, AActor* PlayerStart)
{
	if (Controller && PlayerStart)
	{	
		//現状のポーンを破棄
		APawn* OldPawn = Controller->GetPawn();
		OldPawn->Destroy();

		//スポーンさせるリファレンス
		FString path = "/Script/MyProject.MyPawn";
		//リファレンスからスポーンするクラスを取得
		TSubclassOf<class AMyPawn> spaw_class;//TSubclassOfは、Class型の安全性を提供するテンプレートクラス
		spaw_class = TSoftClassPtr<AActor>(FSoftObjectPath(*path)).LoadSynchronous();

		// ワールドにアクターをスポーン
		APawn* NewPawn = GetWorld()->SpawnActor<AMyPawn>(spaw_class, PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation());
		Controller->Possess(NewPawn);
		
	}
}