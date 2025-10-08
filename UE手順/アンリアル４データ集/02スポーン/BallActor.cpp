// Fill out your copyright notice in the Description page of Project Settings.


#include "BallActor.h"

// Sets default values
ABallActor::ABallActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//���[�g�R���|�[�l���g�쐬
	mRootCollider = CreateDefaultSubobject<USceneComponent>("RootCollider");
	RootComponent = Cast<USceneComponent>(mRootCollider);//���̃A�N�^�[��RootComponent�Ƃ��Đݒ�

	//�X�^�e�B�b�N���b�V���R���|�[�l���g�쐬
	mCubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mCubeMesh->SetupAttachment(RootComponent);//RootComponent�̎q�ɂ���
	//�X�^�e�B�b�N���b�V���u�X�t�B�A�r�W���A���v�̃A�Z�b�g�o�^
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Engine/BasicShapes/Sphere"));
	mCubeMesh->SetStaticMesh(CubeVisualAsset.Object);//�o�^
	mCubeMesh->SetSimulatePhysics(true);  // �����V�~�����[�V������L���ɂ���


	InitialLifeSpan = 3.0f;//�A�N�^�[�̐�������
}

// Called when the game starts or when spawned
void ABallActor::BeginPlay()
{
	Super::BeginPlay();
	//�͂ŉ�����
	mCubeMesh->AddForce(FVector(1, 0, 0) * 400000);
	
}

// Called every frame
void ABallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

