// Fill out your copyright notice in the Description page of Project Settings.


#include "PPawn.h"

// Sets default values
APPawn::APPawn()
{
	//ポーンのスポーン時、PlayerController が自動的にポーンを所有するかを決定（Player0を設定）
	AutoPossessPlayer = EAutoReceiveInput::Player0;

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ルートコンポーネント作成
	mRootCollider = CreateDefaultSubobject<USceneComponent>("RootCollider");
	RootComponent = Cast<USceneComponent>(mRootCollider);//このアクターのRootComponentとして設定

	//スタティックメッシュコンポーネント作成
	mCubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mCubeMesh->SetupAttachment(RootComponent);//RootComponentの子にする
	//スタティックメッシュ「キューブビジュアル」のアセット登録
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Engine/BasicShapes/Cube"));
	mCubeMesh->SetStaticMesh(CubeVisualAsset.Object);//登録
	mCubeMesh->SetSimulatePhysics(true);  // 物理シミュレーションを有効にする

	//カメラコンポーネント作成
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mCamera->SetupAttachment(mCubeMesh);//mCubeMeshの子になる
	mCamera->SetRelativeLocation(FVector( -200.0f,  0.0f, 200.0f));//(x,y,z高さ)　カメラ位置
	mCamera->SetRelativeRotation(FRotator(-20.0f,  0.0f, 0.0f));//カメラの回転角度

	//フローリングポーンムーブコンポーネント作成
	m_PawnMove = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMove"));
	m_PawnMove->UpdatedComponent = mCubeMesh;//移動や更新するするコンポーネントを設定
	//m_PawnMove->bAutoRegisterUpdatedComponent=true;//所有するアクターのルートにUpdatedComponentする
	
}

// Called when the game starts or when spawned
void APPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MOVEX", this, &APPawn::Move_XAxis);
	InputComponent->BindAxis("MOVEY", this, &APPawn::Move_YAxis);
}

//移動メソッド
void APPawn::Move_XAxis(float AxisValue)
{
	
	if (AxisValue != 0.0f)
	{
		//UE_LOG(LogTemp, Log, TEXT("aaa"));
		m_PawnMove->AddInputVector(GetActorRightVector()* AxisValue, false);//ActorRightVector方向に移動
	}
	
}
void APPawn::Move_YAxis(float AxisValue)
{
	
	if (AxisValue != 0.0f)
	{
		//UE_LOG(LogTemp, Log, TEXT("bbb"));
		m_PawnMove->AddInputVector(GetActorForwardVector()* AxisValue, false);//ActorForwardVector方向に移動
	
	}
}

