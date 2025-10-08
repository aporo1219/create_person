// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProjectGameModeBase.h"
#include "MyPawn.h"
#include "DeathActor.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ADeathActor::ADeathActor()
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
	static ConstructorHelpers::FObjectFinder<UStaticMesh> VisualAsset(TEXT("/Engine/BasicShapes/Cone"));
	mStaticMesh->SetStaticMesh(VisualAsset.Object);//登録
	//コリジョン設定
	mStaticMesh->BodyInstance.SetCollisionProfileName("Projectile");//コリジョンプリセット
	mStaticMesh->SetNotifyRigidBodyCollision(true);//コリジョンのHitイベント発行有効（Simulation Generates Hit EventsフラグON）
	mStaticMesh->OnComponentHit.AddDynamic(this, &ADeathActor::OnHit);//ヒットした時の振る舞いするメソッドセット
}

// Called when the game starts or when spawned
void ADeathActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADeathActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ADeathActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//クレイマンデータの確認
	AMyPawn* p = Cast<AMyPawn>(OtherActor);
	if (p != nullptr)
	{
		if ( p->hit==false)
		{
			//カプセルに対して
			p->mCapsule->BodyInstance.SetCollisionProfileName("NoCollision");//コリジョンセット
			p->mCapsule->SetSimulatePhysics(false);  // 物理シミュレーションを無効にする
			p->mCapsule->SetNotifyRigidBodyCollision(false);//コリジョン無効
			p->mCapsule->RecreatePhysicsState();//物理エンジンに対してのコンポーネントの物理状態を再作成

			//メッシュに対して
			p->mSMesh->SetSimulatePhysics(true);  // 物理シミュレーションを有効にする
			p->mSMesh->SetNotifyRigidBodyCollision(true);
			p->mSMesh->BodyInstance.SetCollisionProfileName("BlockAllDynamic");//コリジョンセット
			p->mSMesh->BodyInstance.SetInstanceNotifyRBCollision(true);
			p->mSMesh->BodyInstance.bLockXTranslation = true;//コンストレイント　XY平行移動ロック
			p->mSMesh->BodyInstance.bLockYTranslation = true;
			
			p->mSMesh->RecreatePhysicsState();//物理エンジンに対してのコンポーネントの物理状態を再作成
			p->mSMesh->UpdateComponentToWorld();//コンポーネントからワールドへの変換の値を再計算

			//遅延処理
			UWorld* World = GetWorld();
			FTimerHandle _TimerHandle;
			World->GetTimerManager().SetTimer(_TimerHandle, this, &ADeathActor::HitGameOver, 3.0f,false);
			
						
			p->hit = true;
		}
	}

}

void ADeathActor::HitGameOver()
{
	AMyProjectGameModeBase* GameMode = Cast<AMyProjectGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->ReStart();//ゲームモード内のメソッドをコール
	}
}