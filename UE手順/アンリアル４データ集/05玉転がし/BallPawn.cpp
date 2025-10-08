// Fill out your copyright notice in the Description page of Project Settings.


#include "BallPawn.h"
#include "Kismet/KismetSystemLibrary.h"//PrintString
// Sets default values
ABallPawn::ABallPawn()
{
	//Player0�ɐݒ�
	AutoPossessPlayer = EAutoReceiveInput::Player0;

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//-----�N���G�C�g�R���|�[�l���g-----
	//���[�g�R���|�[�l���g�쐬
	mRootCollider = CreateDefaultSubobject<USceneComponent>("RootCollider");
	RootComponent = Cast<USceneComponent>(mRootCollider);//���̃A�N�^�[��RootComponent�Ƃ��Đݒ�

	//�X�^�e�B�b�N���b�V���R���|�[�l���g�쐬
	mCubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//�X�^�e�B�b�N���b�V���u�X�t�B�A�r�W���A���v�̃A�Z�b�g�o�^
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Engine/BasicShapes/Sphere"));
	mCubeMesh->SetStaticMesh(CubeVisualAsset.Object);//�o�^
	mCubeMesh->SetSimulatePhysics(true);  // �����V�~�����[�V������L���ɂ���
	mCubeMesh->BodyInstance.SetCollisionProfileName("Projectile");//�R���W�����v���Z�b�g
	mCubeMesh->SetNotifyRigidBodyCollision(true);//�R���W������Hit�C�x���g���s�L���iSimulation Generates Hit Events�t���OON�j
	mCubeMesh->OnComponentHit.AddDynamic(this, &ABallPawn::OnHit);//�q�b�g�������̐U�镑�����郁�\�b�h�Z�b�g
	//�X�v�����O�A�[���R���|�[�l���g
	mSpringArm= CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	mSpringArm->TargetArmLength=400.0f;
	mSpringArm->bInheritPitch = false;//Pitch�ERoll�EYaw���I�t���ăJ������]�𖳂���
	mSpringArm->bInheritYaw = false;
	mSpringArm->bInheritRoll = false;
	mSpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));

	//�J�����R���|�[�l���g�쐬
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	
	//-----���[�g-----
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
	//�f�t�H���g�R���g���[�����̃}�b�s���O
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

//�Փ˃��\�b�h
void ABallPawn::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UKismetSystemLibrary::PrintString(this, "aaa", true, true, FColor::Cyan, 2.f);

	//�Փ˂����ʂ̕�����Z���̎�
	float HitDirection = Hit.Normal.Z;

	if (HitDirection > 0)
		mJumpCount = 0;

}
