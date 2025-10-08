// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveBlockActor.h"
#include "MyPawn.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AMoveBlockActor::AMoveBlockActor()
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
	//物理マテリアルアセット登録
	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PMatAsset(TEXT("/Game/NewPhysicalMaterial.NewPhysicalMaterial"));
	if (PMatAsset.Succeeded())
		mStaticMesh->SetPhysMaterialOverride(PMatAsset.Object);//カプセルコンポーネントに物理マテリアルを設定
	//コリジョン設定
	mStaticMesh->BodyInstance.SetCollisionProfileName("Projectile");//コリジョンプリセット
	mStaticMesh->SetNotifyRigidBodyCollision(true);//コリジョンのHitイベント発行有効（Simulation Generates Hit EventsフラグON）
	mStaticMesh->OnComponentHit.AddDynamic(this, &AMoveBlockActor::OnHit);//ヒットした時の振る舞いするメソッドセット
	mStaticMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);//他のオブジェクトとの衝突を検出するだけで、実際の物理的な影響を受けない

}

// Called when the game starts or when spawned
void AMoveBlockActor::BeginPlay()
{
	Super::BeginPlay();

	mMoveTime = 0.0f;				//タイムの初期化
	mLocation = GetActorLocation();//現在の位置情報取得
}

// Called every frame
void AMoveBlockActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	mMoveTime += 0.005f;
	//円運動
	float x = FMath::Sin(mMoveTime) * 300;
	float y = FMath::Cos(mMoveTime) * 300;
	FVector pos(x, y, x);
	SetActorLocation(pos+ mLocation);//新しい位置としてセットする
}

//ヒット
void AMoveBlockActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//クレイマンデータの確認
	AMyPawn* p= Cast<AMyPawn>(OtherActor);
	if (p != nullptr)
	{
		//上方向移動関しては、物理移動のZ値を0にしてジャンプとの違う事を出してる
		FVector vec= p->mCapsule->GetPhysicsLinearVelocity();
		if (vec.Z > 1)
			p->mCapsule->SetPhysicsLinearVelocity(FVector(vec.X, vec.Y, 0));
		
	}

}

