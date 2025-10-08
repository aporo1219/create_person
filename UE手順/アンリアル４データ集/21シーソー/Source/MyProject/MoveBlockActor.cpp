// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveBlockActor.h"
#include "MyPawn.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AMoveBlockActor::AMoveBlockActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//���[�g�R���|�[�l���g
	mRoot = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Cast<USceneComponent>(mRoot);

	//�X�^�e�B�b�N���b�V���R���|�[�l���g�쐬
	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mStaticMesh->SetupAttachment(mRoot);

	//���f���A�Z�b�g�o�^
	static ConstructorHelpers::FObjectFinder<UStaticMesh> VisualAsset(TEXT("/Engine/BasicShapes/Cube"));
	mStaticMesh->SetStaticMesh(VisualAsset.Object);//�o�^
	//�����}�e���A���A�Z�b�g�o�^
	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PMatAsset(TEXT("/Game/NewPhysicalMaterial.NewPhysicalMaterial"));
	if (PMatAsset.Succeeded())
		mStaticMesh->SetPhysMaterialOverride(PMatAsset.Object);//�J�v�Z���R���|�[�l���g�ɕ����}�e���A����ݒ�
	//�R���W�����ݒ�
	mStaticMesh->BodyInstance.SetCollisionProfileName("Projectile");//�R���W�����v���Z�b�g
	mStaticMesh->SetNotifyRigidBodyCollision(true);//�R���W������Hit�C�x���g���s�L���iSimulation Generates Hit Events�t���OON�j
	mStaticMesh->OnComponentHit.AddDynamic(this, &AMoveBlockActor::OnHit);//�q�b�g�������̐U�镑�����郁�\�b�h�Z�b�g
	mStaticMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);//���̃I�u�W�F�N�g�Ƃ̏Փ˂����o���邾���ŁA���ۂ̕����I�ȉe�����󂯂Ȃ�

}

// Called when the game starts or when spawned
void AMoveBlockActor::BeginPlay()
{
	Super::BeginPlay();

	mMoveTime = 0.0f;				//�^�C���̏�����
	mLocation = GetActorLocation();//���݂̈ʒu���擾
}

// Called every frame
void AMoveBlockActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	mMoveTime += 0.005f;
	//�~�^��
	float x = FMath::Sin(mMoveTime) * 300;
	float y = FMath::Cos(mMoveTime) * 300;
	FVector pos(x, y, x);
	SetActorLocation(pos+ mLocation);//�V�����ʒu�Ƃ��ăZ�b�g����
}

//�q�b�g
void AMoveBlockActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//�N���C�}���f�[�^�̊m�F
	AMyPawn* p= Cast<AMyPawn>(OtherActor);
	if (p != nullptr)
	{
		//������ړ��ւ��ẮA�����ړ���Z�l��0�ɂ��ăW�����v�Ƃ̈Ⴄ�����o���Ă�
		FVector vec= p->mCapsule->GetPhysicsLinearVelocity();
		if (vec.Z > 1)
			p->mCapsule->SetPhysicsLinearVelocity(FVector(vec.X, vec.Y, 0));
		
	}

}

