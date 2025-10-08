// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Animation/AnimBlueprint.h"
#include "GameFramework/PlayerInput.h"
#include "MyAnimInstance.h"

// Sets default values
AMyPawn::AMyPawn()
{
	//ポーンのスポーン時、PlayerController が自動的にポーンを所有するかを決定（Player0を設定）
	AutoPossessPlayer = EAutoReceiveInput::Player0;

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ルートコンポーネント作成
	mRootCollider = CreateDefaultSubobject<USceneComponent>("RootCollider");
	RootComponent = Cast<USceneComponent>(mRootCollider);//このアクターのRootComponentとして設定

	//スケルタルメッシュコンポーネント作成
	mSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SMesh"));
	mSMesh->SetupAttachment(RootComponent);//RootComponentの子にする
	
										   
	//スケルタルメッシュ登録
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> VisualAsset(TEXT("/Game/XBot.XBot"));
	mSMesh->SetSkeletalMesh(VisualAsset.Object);//登録

	//リファレンスからスポーンするクラスを取得
	FString path = "/Game/NewAnimBlueprint.NewAnimBlueprint_C";
	TSubclassOf<class UMyAnimInstance> sclass;//TSubclassOfは、Class型の安全性を提供するテンプレートクラス
	sclass = TSoftClassPtr<UMyAnimInstance>(FSoftObjectPath(*path)).LoadSynchronous();
	mSMesh->SetAnimInstanceClass(sclass);//アニムインスタンス登録


	//mSMesh->SetSimulatePhysics(true);  // 物理シミュレーションを有効にする
	//mSMesh->BodyInstance.SetCollisionProfileName("Pawn");//コリジョンセット

	//カメラコンポーネント作成
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mCamera->SetupAttachment(mSMesh);//mCubeMeshの子になる
	mCamera->SetRelativeLocation(FVector(-200.0f, 0.0f, 200.0f));//(x,y,z高さ)　カメラ位置
	mCamera->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));//カメラの回転角度

	//フローリングポーンムーブコンポーネント作成
	m_PawnMove = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMove"));
	m_PawnMove->UpdatedComponent = mSMesh;//移動や更新するするコンポーネントを設定
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	//デフォルトコントローラ情報のマッピング
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MOVEX", EKeys::A, -1));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MOVEX", EKeys::D, 1));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MOVEY", EKeys::W, 1));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MOVEY", EKeys::S, -1));
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MOVEX", this, &AMyPawn::Move_XAxis);
	InputComponent->BindAxis("MOVEY", this, &AMyPawn::Move_YAxis);
}

//移動メソッド
void AMyPawn::Move_XAxis(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		m_PawnMove->AddInputVector(GetActorRightVector() * AxisValue, false);//ActorRightVector方向に移動
		IsWalk = true;
		Speed += 5.0f;
		if (Speed >= 100.0f)
			Speed = 100.0f;
	}

	Speed -= 2.0f;
	if (Speed < 0.0f)
	{
		Speed = 0.0f;
		IsWalk = false;
	}
	

}
void AMyPawn::Move_YAxis(float AxisValue)
{

	if (AxisValue != 0.0f)
	{
		m_PawnMove->AddInputVector(GetActorForwardVector() * AxisValue, false);//ActorForwardVector方向に移動
	}
}