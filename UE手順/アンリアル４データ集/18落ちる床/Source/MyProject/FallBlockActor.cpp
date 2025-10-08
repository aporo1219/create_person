// Fill out your copyright notice in the Description page of Project Settings.

#include "PhysicalMaterials/PhysicalMaterial.h" //物理マテリアル
#include "Kismet/KismetSystemLibrary.h"
#include "FallBlockActor.h"

// Sets default values
AFallBlockActor::AFallBlockActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//スタティックメッシュコンポーネント作成
	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = mStaticMesh;

	//モデルアセット登録
	static ConstructorHelpers::FObjectFinder<UStaticMesh> VisualAsset(TEXT("/Engine/BasicShapes/Cube"));
	mStaticMesh->SetStaticMesh(VisualAsset.Object);//登録
	
	//物理マテリアルアセット登録
	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PMatAsset(TEXT("/Game/NewPhysicalMaterial.NewPhysicalMaterial"));
	if (PMatAsset.Succeeded())
		mStaticMesh->SetPhysMaterialOverride(PMatAsset.Object);//カプセルコンポーネントに物理マテリアルを設定
	
	//オーバーラップ用UBoxComponent* mBox
	mBox = CreateDefaultSubobject<UBoxComponent>("Box");
	mBox->SetupAttachment(mStaticMesh);

}

// Called when the game starts or when spawned
void AFallBlockActor::BeginPlay()
{
	Super::BeginPlay();
	
	mStaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);//他のオブジェクトとの衝突を検出するだけで、実際の物理的な影響を受けない

	
	//オーバーラップ用UBoxComponent* mBoxの設定
	mBox->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));//コリジョンBOXの初期オフセット位置
	mBox->SetGenerateOverlapEvents(true);
	mBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	mBox->OnComponentBeginOverlap.AddDynamic(this, &AFallBlockActor::OverlapBegin);
	mBox->OnComponentEndOverlap.AddDynamic(this, &AFallBlockActor::OverlapEnd);
	mBox->SetHiddenInGame(false);  //ゲーム中に隠さない
	mBox->SetVisibility(true);     //可視化する
	mBox->ShapeColor = FColor::Red;// デバッグ用の色を赤に設定


	//落下タイム
	mTime = 300;
	mTimeSwitch = false;
}

// Called every frame
void AFallBlockActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mTime == -100)
		return;

	//オーバーラップの状態でタイムの増減をする
	if (mTimeSwitch == true)
		mTime--;
	else
		mTime++;

	if (mTime >= 300)
		mTime = 300;

	//タイム0で落下
	if (mTime <= 0)
	{
		mTime = -100;
		//ブロックを落下させ使用不可にする
		mStaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		mStaticMesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
		mStaticMesh->SetSimulatePhysics(true);	// 物理シミュレーションを有効にする
		mStaticMesh->RecreatePhysicsState();//物理エンジンに対してのコンポーネントの物理状態を再作成
	}
	
}

//キャラクターとオーバーラップ（着いた時）
void AFallBlockActor::OverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	mTimeSwitch = true;
}

//キャラクターとオーバーラップ（離れた時）
void AFallBlockActor::OverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	mTimeSwitch = false;
}



