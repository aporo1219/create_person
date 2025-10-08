// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyActor.h"
#include "MyPawn.h"
#include "EnemyAIController.h"
#include "MyProjectGameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionSystem.h"
#include "GameFramework/Pawn.h"


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


	//追加内容：AIによる追尾
	mAIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	
	// 視覚設定
	mSightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	mSightConfig->SightRadius = 2000.0f;  // 視覚の範囲
	mSightConfig->LoseSightRadius = 2200.0f;  // 見失う範囲
	mSightConfig->PeripheralVisionAngleDegrees = 60.0f;  // 視野角

	mSightConfig->DetectionByAffiliation.bDetectEnemies = true;
	mSightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	mSightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	
	mSightConfig->AutoSuccessRangeFromLastSeenLocation = 1;//一時的に見失っても範囲内であれば感知する
	mSightConfig->SetMaxAge(0.1f);//感知を忘れられるまでの時間
	
	mAIPerception->ConfigureSense(*mSightConfig);//感覚のセット
	mAIPerception->SetDominantSense(UAISense_Sight::StaticClass());//感覚の優先クラス設定
	
	// 視覚イベントの設定
	mAIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyActor::OnPerceptionUpdated);
	

	
	//追加内容：AIによる追尾
	// AIControllerを設定する
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;  // ゲーム内でスポーンしたときにAIControllerを自動でアタッチ
	AIControllerClass = AEnemyAIController::StaticClass();   // AIControllerのクラスを指定
}

// Called when the game starts or when spawned
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();
	
	mSMesh->SetRelativeLocation(FVector(0, 0, -40));//スケルタルメッシュの初期オフセット位置
	mBox->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));//コリジョンBOXの初期オフセット位置

	
	
	
	//SpawnDefaultController();//登録してるAIControllerを生成

	 // 速度の設定
	if (mMove)
	{
		mMove->MaxSpeed     = 200.0f;    // 最大速度
		mMove->Acceleration = 2048.0f;  // 加速度
		mMove->Deceleration = 2048.0f;  // 減速度
	}

	//姿勢クォータニオンの初期化
	mNowQ = FQuat(FVector(0, 0, 1), 0.0f);
	mNewQ = FQuat(FVector(0, 0, 1), 0.0f);

}

// Called every frame
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//追加内容：AIによる追尾
	FVector wpos;
	FRotator wrot;
	wrot = mSMesh->GetRelativeRotation();
	wpos = mCapsule->GetRelativeLocation();
	mRoot->SetRelativeRotation(wrot+FRotator(0,90,0));
	mRoot->SetRelativeLocation(wpos);

	//移動方向にモデルの向きを変える
	FVector mMoveVec = mMove->Velocity;//フローリングムーブコンテントから移動ベクトルを獲得
	mMoveVec.Z = 0.0f;//XY軸ベクトルにする
	mMoveVec.Normalize();

	//新しい姿勢を獲得（MyPawnからコピペ）
	if (mMoveVec.Size() > 0.1f)
	{
		FVector w(0, 1, 0);
		float r = FVector::DotProduct(w, mMoveVec);			//wベクトルを0度として回転角度
		FVector rvec = FVector::CrossProduct(w, mMoveVec);	///wベクトルと移動方向から回転軸ベクトル求める
		rvec.Normalize();//回転軸ベクトルを正規化

		//法線の場合、不可ベクトルが発生する場合がある
		if (rvec.IsNearlyZero())
			rvec = FVector(0, 0, 1);

		//回転角度と回転軸ベクトルからクォータニオンを取得
		mNewQ = FQuat(rvec, acos(r));
	}

	//現在の姿勢と新しい姿勢を補間
	mNowQ = FQuat::Slerp(mNowQ, mNewQ, 0.1f);

	//姿勢向きに回転させる(　衝突時：モデルを物理シミュレーションするのでテレポートフラグをONにする　)
	mSMesh->SetRelativeRotation(mNowQ, false, nullptr, ETeleportType::ResetPhysics);
	
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
	
	if (HitDirection <= -0.5)
	{
		//コンポーネントのパーティクルを独立して実行
		UParticleSystem* ParticleTemplate = mParticle->Template;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleTemplate, GetActorLocation(), GetActorRotation());
		Destroy();//アクター削除
	}
}

//追加内容：AIによる追尾
void AEnemyActor::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{ 
	
	if (Stimulus.WasSuccessfullySensed())
	{
		// 視覚でActorを発見した時の処理
		//視覚で見つけたプレイヤー
		AMyPawn* player = Cast<AMyPawn>(&Actor[0]);
		//AIコントローラーの確認
		AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
		
		// AIControllerにプレイヤー情報を設定
		if (AIController && player)
		{
			AIController->SetPlayerKey(player);
		}
	}
	else
	{
		// 視覚から外れた時の処理
	}
	
}

