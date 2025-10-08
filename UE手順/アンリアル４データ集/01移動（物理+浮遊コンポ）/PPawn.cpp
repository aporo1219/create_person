// Fill out your copyright notice in the Description page of Project Settings.


#include "PPawn.h"

// Sets default values
APPawn::APPawn()
{
	//�|�[���̃X�|�[�����APlayerController �������I�Ƀ|�[�������L���邩������iPlayer0��ݒ�j
	AutoPossessPlayer = EAutoReceiveInput::Player0;

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//���[�g�R���|�[�l���g�쐬
	mRootCollider = CreateDefaultSubobject<USceneComponent>("RootCollider");
	RootComponent = Cast<USceneComponent>(mRootCollider);//���̃A�N�^�[��RootComponent�Ƃ��Đݒ�

	//�X�^�e�B�b�N���b�V���R���|�[�l���g�쐬
	mCubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mCubeMesh->SetupAttachment(RootComponent);//RootComponent�̎q�ɂ���
	//�X�^�e�B�b�N���b�V���u�L���[�u�r�W���A���v�̃A�Z�b�g�o�^
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Engine/BasicShapes/Cube"));
	mCubeMesh->SetStaticMesh(CubeVisualAsset.Object);//�o�^
	mCubeMesh->SetSimulatePhysics(true);  // �����V�~�����[�V������L���ɂ���

	//�J�����R���|�[�l���g�쐬
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mCamera->SetupAttachment(mCubeMesh);//mCubeMesh�̎q�ɂȂ�
	mCamera->SetRelativeLocation(FVector( -200.0f,  0.0f, 200.0f));//(x,y,z����)�@�J�����ʒu
	mCamera->SetRelativeRotation(FRotator(-20.0f,  0.0f, 0.0f));//�J�����̉�]�p�x

	//�t���[�����O�|�[�����[�u�R���|�[�l���g�쐬
	m_PawnMove = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMove"));
	m_PawnMove->UpdatedComponent = mCubeMesh;//�ړ���X�V���邷��R���|�[�l���g��ݒ�
	//m_PawnMove->bAutoRegisterUpdatedComponent=true;//���L����A�N�^�[�̃��[�g��UpdatedComponent����
	
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

//�ړ����\�b�h
void APPawn::Move_XAxis(float AxisValue)
{
	
	if (AxisValue != 0.0f)
	{
		//UE_LOG(LogTemp, Log, TEXT("aaa"));
		m_PawnMove->AddInputVector(GetActorRightVector()* AxisValue, false);//ActorRightVector�����Ɉړ�
	}
	
}
void APPawn::Move_YAxis(float AxisValue)
{
	
	if (AxisValue != 0.0f)
	{
		//UE_LOG(LogTemp, Log, TEXT("bbb"));
		m_PawnMove->AddInputVector(GetActorForwardVector()* AxisValue, false);//ActorForwardVector�����Ɉړ�
	
	}
}

