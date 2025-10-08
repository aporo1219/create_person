// Fill out your copyright notice in the Description page of Project Settings.


#include "PPawn.h"
#include "BallActor.h"
#include "GameFramework/PlayerInput.h"

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
	//デフォルトコントローラ情報のマッピング
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MOVEX", EKeys::A,-1));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MOVEX", EKeys::D, 1));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MOVEY", EKeys::W, 1));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MOVEY", EKeys::S,-1));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("FIRE", EKeys::LeftMouseButton));

	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis("MOVEX", this, &APPawn::Move_XAxis);
	InputComponent->BindAxis("MOVEY", this, &APPawn::Move_YAxis);
	PlayerInputComponent->BindAction("FIRE", IE_Pressed, this, &APPawn::Key_Attack);

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


void APPawn::Key_Attack()
{
	//スポーンさせるリファレンス
	FString path = "/Script/MyProject.BallActor";
	//リファレンスからスポーンするクラスを取得
	TSubclassOf<class ABallActor> spaw_class;//TSubclassOfは、Class型の安全性を提供するテンプレートクラス
	spaw_class= TSoftClassPtr<AActor>(FSoftObjectPath(*path)).LoadSynchronous();

	//ワールド獲得
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		// ワールドにアクターをスポーン
		ABallActor* p= World->SpawnActor<ABallActor>(spaw_class);
		if (p != nullptr)
		{
			//アクターのロケーション(位置)を設定
			p->SetActorLocation(FVector(0, 0, 10));
		}
	}
}

