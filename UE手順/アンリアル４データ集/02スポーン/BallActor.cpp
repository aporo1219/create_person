// Fill out your copyright notice in the Description page of Project Settings.


#include "BallActor.h"

// Sets default values
ABallActor::ABallActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ルートコンポーネント作成
	mRootCollider = CreateDefaultSubobject<USceneComponent>("RootCollider");
	RootComponent = Cast<USceneComponent>(mRootCollider);//このアクターのRootComponentとして設定

	//スタティックメッシュコンポーネント作成
	mCubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mCubeMesh->SetupAttachment(RootComponent);//RootComponentの子にする
	//スタティックメッシュ「スフィアビジュアル」のアセット登録
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Engine/BasicShapes/Sphere"));
	mCubeMesh->SetStaticMesh(CubeVisualAsset.Object);//登録
	mCubeMesh->SetSimulatePhysics(true);  // 物理シミュレーションを有効にする


	InitialLifeSpan = 3.0f;//アクターの生存時間
}

// Called when the game starts or when spawned
void ABallActor::BeginPlay()
{
	Super::BeginPlay();
	//力で加える
	mCubeMesh->AddForce(FVector(1, 0, 0) * 400000);
	
}

// Called every frame
void ABallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

