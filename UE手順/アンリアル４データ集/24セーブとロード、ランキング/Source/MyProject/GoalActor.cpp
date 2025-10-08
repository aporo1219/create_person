// Fill out your copyright notice in the Description page of Project Settings.

#include "GlobalData.h"
#include "MySaveGame.h"
#include "GoalActor.h"
#include "GameFramework/PlayerStart.h"
#include "MyProjectGameModeBase.h"
#include "MyPawn.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGoalActor::AGoalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ルートコンポーネント
	mRoot = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Cast<USceneComponent>(mRoot);

	//スタティックメッシュコンポーネント作成
	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mStaticMesh->SetupAttachment(mRoot);

	
	//モデルアセット登録
	static ConstructorHelpers::FObjectFinder<UStaticMesh> VisualAsset(TEXT("/Engine/BasicShapes/Cube"));
	mStaticMesh->SetStaticMesh(VisualAsset.Object);//登録


	

}

// Called when the game starts or when spawned
void AGoalActor::BeginPlay()
{
	Super::BeginPlay();
	//コリジョン
	mStaticMesh->SetGenerateOverlapEvents(true);//オーバーラップイベントをtrueにする(GenerateOverlapEventsフラグON)
	mStaticMesh->BodyInstance.SetCollisionProfileName("OverlapAll");//コリジョンプリセット(OverlapAllを設定)
	mStaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AGoalActor::OverlapBegin);//オーバーラップスタート時イベント


	
	//※24セーブとロード、ランキングでの追加
	//GameInstanceのグローバルデータのアドレス獲得
	AMyProjectGameModeBase* GameMode = Cast<AMyProjectGameModeBase>(GetWorld()->GetAuthGameMode());
	UGlobalData* instance = Cast<UGlobalData>(GetGameInstance());
	//ファイル名「ScoreGameData」からセーブデータをロードする
	UMySaveGame* saveData =Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot("ScoreGameData", 0));
	if (saveData != nullptr)
	{
		//ロードしたデータをグローバルデータに入れる
		int32* pp = instance->GetData();
		for (int i = 0; i < 5; i++)
			pp[i] = saveData->Score[i];

		saveData->MarkPendingKill();
	}
}

// Called every frame
void AGoalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//オーバーラップビギンメソッド
void AGoalActor::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	//クレイマンデータの確認（ラムダで使用する為、アドレスを持っておくためpはメンバ）
	p = Cast<AMyPawn>(OtherActor);
	if (p != nullptr)
	{
		//※24セーブとロード、ランキングでの追加
		//GameInstanceのグローバルデータにデータを登録
		AMyProjectGameModeBase* GameMode = Cast<AMyProjectGameModeBase>(GetWorld()->GetAuthGameMode());
		UGlobalData* instance = Cast<UGlobalData>(GetGameInstance());
		
		if (GameMode && instance)
		{
			instance->SetData(GameMode->mPoint);
		}

		//
		// UMySaveGameのインスタンスを生成.
		UMySaveGame* saveData = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
		ensureMsgf(saveData, TEXT("saveData is null."));

		if (saveData!=nullptr)
		{
			int32* pp=instance->GetData();
			for (int i = 0; i < 5; i++)
				saveData->Score[i] = pp[i];
			//データセーブ
			UGameplayStatics::SaveGameToSlot(saveData, saveData->SlotNameGameData, sizeof(int32)*5);
			saveData->MarkPendingKill();
		}
	
		

		//ゴール認識
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Goal")), true, true, FColor::Cyan, 2.f);


		// PlayerStart オブジェクトを見つける (PlayerStartTagの "Respawn" 検索)
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
		
		AActor* ChosenPlayerStart = nullptr;
		for (AActor* PlayerStart : PlayerStarts)
		{
			APlayerStart* ps = Cast<APlayerStart>(PlayerStart);
			if (ps != nullptr)
			{
				if (ps->PlayerStartTag == FName("Respawn"))
				{
					ChosenPlayerStart = PlayerStart;
					break;
				}
			}
		}
		
	
		// 位置を設定
		p->mCapsule->SetWorldLocation(ChosenPlayerStart->GetActorLocation(),false,nullptr, ETeleportType::ResetPhysics);
	
		//位置変更による移動力発生を抑制するため一時的に動かないようにする
		p->mCapsule->BodyInstance.bLockXTranslation = true;
		p->mCapsule->BodyInstance.bLockYTranslation = true;
		p->mCapsule->BodyInstance.bLockZTranslation = true;
		p->mCapsule->RecreatePhysicsState();

		//遅延処理(命令はラムダで記載)
		UWorld*  World = GetWorld();
		FTimerHandle _TimerHandle;
		World->GetTimerManager().SetTimer(_TimerHandle, [&]()
			{
				//位置変更による移動力発生はタイミングが違うので、遅延させて移動力を0にする
				p->mMove->Velocity = FVector::ZeroVector;
				p->mCapsule->BodyInstance.bLockXTranslation = false;
				p->mCapsule->BodyInstance.bLockYTranslation = false;
				p->mCapsule->BodyInstance.bLockZTranslation = false;
				p->mCapsule->RecreatePhysicsState();
			},
		0.1f, false);

	
	}


}