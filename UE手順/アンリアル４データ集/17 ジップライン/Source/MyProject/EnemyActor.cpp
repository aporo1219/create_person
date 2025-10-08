// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "MyProjectGameModeBase.h"
#include "EnemyActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

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

	//追加内容：頭に踏んだら爆発
	//追加
	mParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	mParticle->SetupAttachment(mSMesh);
	mParticle->bAutoActivate = false;// 自動アクティブ化を無効にするこれをしないと、登録時に実行される
	
	//　パーティクル登録
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/FXVarietyPack/Particles/P_ky_hit2.P_ky_hit2"));
	if (ParticleAsset.Succeeded())
	{
		mParticle->SetTemplate(ParticleAsset.Object); 
	}


	//UBoxComponent* mBox
	mBox = CreateDefaultSubobject<UBoxComponent>("Box");
	mBox->SetupAttachment(mCapsule);
	//mBox->SetSimulatePhysics(true);					   //物理ON
	mBox->BodyInstance.SetCollisionProfileName("Pawn");//コリジョンセット
	mBox->SetNotifyRigidBodyCollision(true);//コリジョンのHitイベント発行有効（Simulation Generates Hit EventsフラグON）
	mBox->OnComponentHit.AddDynamic(this, &AEnemyActor::OnHitBox);//ヒットした時の振る舞いするメソッドセット
	
	//衝突判定描画デバック用
	mBox->SetHiddenInGame(false);  //ゲーム中に隠さない
	mBox->SetVisibility(true);     //可視化する
	mBox->ShapeColor = FColor::Red;// デバッグ用の色を赤に設定

	mCapsule->SetHiddenInGame(false);
	mCapsule->SetVisibility(true);
	mCapsule->ShapeColor = FColor::Red;
}

// Called when the game starts or when spawned
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();
	mSMesh->SetRelativeLocation(FVector(0, 0, -40));//スケルタルメッシュの初期オフセット位置
	mBox->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));//コリジョンBOXの初期オフセット位置

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

//追加内容：頭に踏んだら爆発
void AEnemyActor::OnHitBox(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//衝突した面の方向がZ軸の時
	float HitDirection = Hit.Normal.Z;

	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("t=%f"), HitDirection), true, true, FColor::Cyan, 2.f);


	if (HitDirection <= -0.5)
	{
		// コンポーネントのパーティクルシステムを実行
		//mParticle->ActivateSystem(); 
		//UKismetSystemLibrary::PrintString(this, TEXT("HIT"), true, true, FColor::Cyan, 2.f);

		//コンポーネントのパーティクルを独立して実行
		UParticleSystem* ParticleTemplate = mParticle->Template;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleTemplate, GetActorLocation(), GetActorRotation());
		Destroy();//アクター削除
	}
}

