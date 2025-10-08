// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "MyProjectGameModeBase.h"
#include "EnemyActor.h"

// Sets default values
AEnemyActor::AEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//USceneComponent* mRoot;//ルートコンポーネント
	mRoot = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Cast<USceneComponent>(mRoot);//このアクターのRootComponentとして設定

	//UCapsuleComponent* mCapsule;	//カプセルコンポーネント
	mCapsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	mCapsule->SetupAttachment(RootComponent);
	mCapsule->SetSimulatePhysics(true);					   //物理ON
	mCapsule->BodyInstance.SetCollisionProfileName("Pawn");//コリジョンセット
	mCapsule->SetNotifyRigidBodyCollision(true);//コリジョンのHitイベント発行有効（Simulation Generates Hit EventsフラグON）
	mCapsule->OnComponentHit.AddDynamic(this, &AEnemyActor::OnHit);//ヒットした時の振る舞いするメソッドセット
	//コンストレイント　回転ロック
	mCapsule->BodyInstance.bLockXRotation = true;
	mCapsule->BodyInstance.bLockYRotation = true;
	mCapsule->BodyInstance.bLockZRotation = true;

	
	//USkeletalMeshComponent* mSMesh;//スケルタルメッシュコンポーネント
	mSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SMesh"));
	mSMesh->SetupAttachment(mCapsule);//RootComponentの子にする
	//スケルタルメッシュ登録
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> VisualAsset(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin_Female.SK_Mannequin_Female"));
	mSMesh->SetSkeletalMesh(VisualAsset.Object);//登録
	//リファレンスからスポーンするクラスを取得
	FString path = "/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C";
	TSubclassOf<class UAnimInstance> sclass;//TSubclassOfは、Class型の安全性を提供するテンプレートクラス
	sclass = TSoftClassPtr<UAnimInstance>(FSoftObjectPath(*path)).LoadSynchronous();
	mSMesh->SetAnimClass((UClass*)sclass);

	//UFloatingPawnMovement* mMove;	//浮遊移動コンポーネント
	mMove = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Move"));
	mMove->UpdatedComponent = mCapsule;//移動や更新するするコンポーネントを設定
}

// Called when the game starts or when spawned
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();
	mSMesh->SetRelativeLocation(FVector(0, 0, -40));//スケルタルメッシュの初期オフセット位置
}

// Called every frame
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//クレイマンデータの確認
	AMyPawn* p = Cast<AMyPawn>(OtherActor);
	if (p != nullptr)
	{
		if (p->hit == false)
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
			World->GetTimerManager().SetTimer(_TimerHandle, [&]()
			{
				//ゲームモード内のメソッドをコール
				AMyProjectGameModeBase* GameMode = Cast<AMyProjectGameModeBase>(GetWorld()->GetAuthGameMode());
				if (GameMode)
					GameMode->ReStart();
			}, 3.0f, false);


			p->hit = true;
		}
	}

}


