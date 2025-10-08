// Copyright Epic Games, Inc. All Rights Reserved.


#include "MyProjectGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.h"
#include "ItemActor.h"
#include "BallPawn.h"

AMyProjectGameModeBase::AMyProjectGameModeBase()
{ 
	//デフォルトポーンクラスをABallPawnに設定
	DefaultPawnClass = ABallPawn::StaticClass();
}

void AMyProjectGameModeBase::BeginPlay()
{
	//スポーンさせるリファレンス
	FString path = "/Script/MyProject.ItemActor";
	//リファレンスからスポーンするクラスを取得
	TSubclassOf<class AItemActor> spaw_class;//TSubclassOfは、Class型の安全性を提供するテンプレートクラス
	spaw_class = TSoftClassPtr<AActor>(FSoftObjectPath(*path)).LoadSynchronous();

	// ワールドにアクターをスポーン
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		AItemActor* p;
		p= World->SpawnActor<AItemActor>(spaw_class);
		p->mCubeMesh->SetRelativeLocation(FVector(0, 0, 70));

		p = World->SpawnActor<AItemActor>(spaw_class);
		p->mCubeMesh->SetRelativeLocation(FVector(250, 0, 75));

		p = World->SpawnActor<AItemActor>(spaw_class);
		p->mCubeMesh->SetRelativeLocation(FVector(0, 250, 80));

		p = World->SpawnActor<AItemActor>(spaw_class);
		p->mCubeMesh->SetRelativeLocation(FVector(250, 250, 85));
	}

	//ワールド内のAItemアクターを探す
	TArray<AActor* > Imtes;
	UGameplayStatics::GetAllActorsOfClass(World, AItemActor::StaticClass(), Imtes);
	ItemsInLevel = Imtes.Num();

	//NewWidgetBlueprintの作成
	path = "/Game/NewWidgetBlueprint.NewWidgetBlueprint_C";
	TSubclassOf<class UUserWidget> GameWigetClass;
	GameWigetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*path)).LoadSynchronous();
	if (GameWigetClass)
	{
		GameWidget = CreateWidget<UMyUserWidget>(GetWorld(), GameWigetClass);
		if (GameWidget)
		{
			//ビューポート描画
			GameWidget->AddToViewport();
			UpdateItemText();
		}

	}

}

void AMyProjectGameModeBase::UpdateItemText()
{
	GameWidget->SetItemText(ItemsCollected, ItemsInLevel);//ウィジェットのデータ更新
}

void AMyProjectGameModeBase::ItemCollected()
{
	ItemsCollected++;
	UpdateItemText();
}
