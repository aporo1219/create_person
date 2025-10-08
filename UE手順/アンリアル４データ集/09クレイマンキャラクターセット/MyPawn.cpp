// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Kismet/KismetSystemLibrary.h"//PrintString
// Sets default values
AMyPawn::AMyPawn()
{
	//ポーンのスポーン時、PlayerController が自動的にポーンを所有するかを決定（Player0を設定）
	AutoPossessPlayer = EAutoReceiveInput::Player0;

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//USceneComponent* mRoot;//ルートコンポーネント
	mRoot = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Cast<USceneComponent>(mRoot);//このアクターのRootComponentとして設定

	//UCapsuleComponent* mCapsule;	//カプセルコンポーネント
	mCapsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	mCapsule->SetupAttachment(RootComponent);
	mCapsule->SetSimulatePhysics(true);					   //物理ON
	mCapsule->BodyInstance.SetCollisionProfileName("Pawn");//コリジョンセット
	//コンストレイント　回転ロック
	mCapsule->BodyInstance.bLockXRotation = true;
	mCapsule->BodyInstance.bLockYRotation = true;
	mCapsule->BodyInstance.bLockZRotation = true;

	//物理マテリアルアセットを探す
	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PMatAsset(TEXT("/Game/NewPhysicalMaterial.NewPhysicalMaterial"));
	if (PMatAsset.Succeeded())
	{
		mCapsule->SetPhysMaterialOverride(PMatAsset.Object);//カプセルコンポーネントに物理マテリアルを設定
	}
	
	

	//USkeletalMeshComponent* mSMesh;//スケルタルメッシュコンポーネント
	mSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SMesh"));
	mSMesh->SetupAttachment(mCapsule);//RootComponentの子にする
	//スケルタルメッシュ登録
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> VisualAsset(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));
	mSMesh->SetSkeletalMesh(VisualAsset.Object);//登録
	//リファレンスからスポーンするクラスを取得
	FString path = "/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C";
	TSubclassOf<class UAnimInstance> sclass;//TSubclassOfは、Class型の安全性を提供するテンプレートクラス
	sclass = TSoftClassPtr<UAnimInstance>(FSoftObjectPath(*path)).LoadSynchronous();
	mSMesh->SetAnimClass((UClass*)sclass);

	

	//USpringArmComponent* mSpringArm スプリングアームコンポーネント
	mSpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	mSpringArm->TargetArmLength = 400.0f;
	mSpringArm->bInheritPitch = false;//Pitch・Roll・Yawをオフしてカメラ回転を無くす
	mSpringArm->bInheritYaw = false;
	mSpringArm->bInheritRoll = false;
	mSpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	mSpringArm->SetupAttachment(mSMesh);

	//UCameraComponent* mCamera; カメラコンポーネント作成
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mCamera->SetupAttachment(mSpringArm);

	//UFloatingPawnMovement* mMove;	//浮遊移動コンポーネント
	mMove = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Move"));
	mMove->UpdatedComponent = mCapsule;//移動や更新するするコンポーネントを設定
	
	
	
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	//移動キー方向ベクトルの初期化
	mMoveVecX = FVector(0, 0, 0);
	mMoveVecZ = FVector(0, 0, 0);
	//姿勢クォータニオンの初期化
	mNowQ = FQuat(FVector(0, 0, 1), 0.0f);
	mNewQ = FQuat(FVector(0, 0, 1), 0.0f);
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//移動方向
	FVector mMoveVec= mMoveVecX + mMoveVecZ;
	mMoveVec.Normalize();

	//移動
	mMove->AddInputVector(mMoveVec, false);

	//新しい姿勢を獲得
	if (mMoveVec.Size() > 0.1f)
	{
		FVector w(0, 1, 0);
		float r = FVector::DotProduct(w, mMoveVec);			//wベクトルを0度として回転角度
		FVector rvec = FVector::CrossProduct(w,mMoveVec);	///wベクトルと移動方向から回転軸ベクトル求める
		rvec.Normalize();//回転軸ベクトルを正規化

		//法線の場合、不可ベクトルが発生する場合がある
		if (rvec.IsNearlyZero())
			rvec = FVector(0, 0, 1);

		//回転角度と回転軸ベクトルからクォータニオンを取得
		mNewQ = FQuat(rvec,acos(r));
	}
	
	//現在の姿勢と新しい姿勢を補間
	mNowQ = FQuat::Slerp(mNowQ, mNewQ, 0.1f);

	//姿勢向きに回転させる
	mSMesh->SetRelativeRotation(mNowQ);

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//デフォルトコントローラ情報のマッピング
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MOVEX", EKeys::A,-1));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MOVEX", EKeys::D, 1));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MOVEY", EKeys::W, 1));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MOVEY", EKeys::S,-1));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("JUMP", EKeys::LeftMouseButton));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MOV_X",EKeys::MouseX,1.0f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MOV_Y", EKeys::MouseY, 1.0f));

	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis("MOVEX", this, &AMyPawn::Move_XAxis);
	InputComponent->BindAxis("MOVEY", this, &AMyPawn::Move_YAxis);
	InputComponent->BindAxis("MOV_X", this, &AMyPawn::CameraMove_XAxis);
	InputComponent->BindAxis("MOV_Y", this, &AMyPawn::CameraMove_YAxis);
	PlayerInputComponent->BindAction("JUMP", IE_Pressed, this, &AMyPawn::Jump);
}

//移動メソッド
void AMyPawn::Move_XAxis(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		FVector vec(1.0f, 1.0f, 0.0);
		mMoveVecX = mCamera->GetRightVector()*vec;//Z軸破棄
		mMoveVecX *= AxisValue;
	}
	else
	{
		mMoveVecX= FVector(0, 0, 0);
	}
}
void AMyPawn::Move_YAxis(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		FVector vec(1.0f, 1.0f, 0.0);
		mMoveVecZ = mCamera->GetForwardVector() * vec;//Z軸破棄
		mMoveVecZ *= AxisValue;
	}
	else
	{
		mMoveVecZ = FVector(0, 0, 0);
	}
}

void AMyPawn::Jump()
{
	FVector vec(0.0f, 0.0f, 1.0);
	mCapsule->AddForce(GetActorUpVector() * 1000000);
	
}

//カメラ回転メソッド
void AMyPawn::CameraMove_XAxis(float Value)
{
	FRotator r = mSpringArm->GetRelativeRotation();
	mSpringArm->AddRelativeRotation(FRotator(0.0f, Value, 0.0));
	
}

void AMyPawn::CameraMove_YAxis(float Value)
{
	FRotator r = mSpringArm->GetRelativeRotation();
	if (r.Pitch + Value < -60.0f || r.Pitch + Value > -10.0f)
	{
		;
	}
	else
	{
		//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("t=%f"),r.Pitch+ Value), true, true, FColor::Cyan, 2.f);
		mSpringArm->AddRelativeRotation(FRotator(Value, 0.0f, 0.0));
	}
}