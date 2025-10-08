// Fill out your copyright notice in the Description page of Project Settings.


#include "BallPawn.h"
#include "Kismet/KismetSystemLibrary.h"//PrintString
// Sets default values
ABallPawn::ABallPawn()
{
	//Player0に設定
	AutoPossessPlayer = EAutoReceiveInput::Player0;

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//-----クリエイトコンポーネント-----
	//ルートコンポーネント作成
	mRootCollider = CreateDefaultSubobject<USceneComponent>("RootCollider");
	RootComponent = Cast<USceneComponent>(mRootCollider);//このアクターのRootComponentとして設定

	//スタティックメッシュコンポーネント作成
	mCubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//スタティックメッシュ「スフィアビジュアル」のアセット登録
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Engine/BasicShapes/Sphere"));
	mCubeMesh->SetStaticMesh(CubeVisualAsset.Object);//登録
	mCubeMesh->SetSimulatePhysics(true);  // 物理シミュレーションを有効にする
	mCubeMesh->BodyInstance.SetCollisionProfileName("Projectile");//コリジョンプリセット
	mCubeMesh->SetNotifyRigidBodyCollision(true);//コリジョンのHitイベント発行有効（Simulation Generates Hit EventsフラグON）
	mCubeMesh->OnComponentHit.AddDynamic(this, &ABallPawn::OnHit);//ヒットした時の振る舞いするメソッドセット
	//スプリングアームコンポーネント
	mSpringArm= CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	mSpringArm->TargetArmLength=400.0f;
	mSpringArm->bInheritPitch = false;//Pitch・Roll・Yawをオフしてカメラ回転を無くす
	mSpringArm->bInheritYaw = false;
	mSpringArm->bInheritRoll = false;
	mSpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));

	//カメラコンポーネント作成
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	
	//-----ルート-----
	mCubeMesh->SetupAttachment(RootComponent);
	mSpringArm->SetupAttachment(mCubeMesh);
	mCamera->SetupAttachment(mSpringArm);

}

// Called when the game starts or when spawned
void ABallPawn::BeginPlay()
{
	Super::BeginPlay();
	mMoveForce = 800000 / mCubeMesh->GetMass();
	mJumpCount = 0;
}

// Called every frame
void ABallPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

// Called to bind functionality to input
void ABallPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//デフォルトコントローラ情報のマッピング
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MOVEX", EKeys::A, -1));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MOVEX", EKeys::D, 1));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MOVEY", EKeys::W, 1));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MOVEY", EKeys::S, -1));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("JUMP", EKeys::LeftMouseButton));

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MOVEX", this, &ABallPawn::MoveRight);
	InputComponent->BindAxis("MOVEY", this, &ABallPawn::MoveForward);
	PlayerInputComponent->BindAction("JUMP", IE_Pressed, this, &ABallPawn::Jump);
}

void ABallPawn::MoveRight(float value)
{
	const FVector Right = mCamera->GetRightVector() * mMoveForce * value;
	mCubeMesh->AddForce(Right);
}
void ABallPawn::MoveForward(float value)
{
	const FVector Forward = mCamera->GetForwardVector() * mMoveForce * value;
	mCubeMesh->AddForce(Forward);
}

void ABallPawn::Jump()
{
	if (mJumpCount >= 1)
	{
		return;
	}
	else
	{
		mJumpCount++;
		mCubeMesh->AddImpulse(FVector(0, 0, 50000));
	}
}

//衝突メソッド
void ABallPawn::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UKismetSystemLibrary::PrintString(this, "aaa", true, true, FColor::Cyan, 2.f);

	//衝突した面の方向がZ軸の時
	float HitDirection = Hit.Normal.Z;

	if (HitDirection > 0)
		mJumpCount = 0;

}
