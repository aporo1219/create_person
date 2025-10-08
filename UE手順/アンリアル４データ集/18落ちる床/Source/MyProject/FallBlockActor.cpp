// Fill out your copyright notice in the Description page of Project Settings.

#include "PhysicalMaterials/PhysicalMaterial.h" //�����}�e���A��
#include "Kismet/KismetSystemLibrary.h"
#include "FallBlockActor.h"

// Sets default values
AFallBlockActor::AFallBlockActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�X�^�e�B�b�N���b�V���R���|�[�l���g�쐬
	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = mStaticMesh;

	//���f���A�Z�b�g�o�^
	static ConstructorHelpers::FObjectFinder<UStaticMesh> VisualAsset(TEXT("/Engine/BasicShapes/Cube"));
	mStaticMesh->SetStaticMesh(VisualAsset.Object);//�o�^
	
	//�����}�e���A���A�Z�b�g�o�^
	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PMatAsset(TEXT("/Game/NewPhysicalMaterial.NewPhysicalMaterial"));
	if (PMatAsset.Succeeded())
		mStaticMesh->SetPhysMaterialOverride(PMatAsset.Object);//�J�v�Z���R���|�[�l���g�ɕ����}�e���A����ݒ�
	
	//�I�[�o�[���b�v�pUBoxComponent* mBox
	mBox = CreateDefaultSubobject<UBoxComponent>("Box");
	mBox->SetupAttachment(mStaticMesh);

}

// Called when the game starts or when spawned
void AFallBlockActor::BeginPlay()
{
	Super::BeginPlay();
	
	mStaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);//���̃I�u�W�F�N�g�Ƃ̏Փ˂����o���邾���ŁA���ۂ̕����I�ȉe�����󂯂Ȃ�

	
	//�I�[�o�[���b�v�pUBoxComponent* mBox�̐ݒ�
	mBox->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));//�R���W����BOX�̏����I�t�Z�b�g�ʒu
	mBox->SetGenerateOverlapEvents(true);
	mBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	mBox->OnComponentBeginOverlap.AddDynamic(this, &AFallBlockActor::OverlapBegin);
	mBox->OnComponentEndOverlap.AddDynamic(this, &AFallBlockActor::OverlapEnd);
	mBox->SetHiddenInGame(false);  //�Q�[�����ɉB���Ȃ�
	mBox->SetVisibility(true);     //��������
	mBox->ShapeColor = FColor::Red;// �f�o�b�O�p�̐F��Ԃɐݒ�


	//�����^�C��
	mTime = 300;
	mTimeSwitch = false;
}

// Called every frame
void AFallBlockActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mTime == -100)
		return;

	//�I�[�o�[���b�v�̏�ԂŃ^�C���̑���������
	if (mTimeSwitch == true)
		mTime--;
	else
		mTime++;

	if (mTime >= 300)
		mTime = 300;

	//�^�C��0�ŗ���
	if (mTime <= 0)
	{
		mTime = -100;
		//�u���b�N�𗎉������g�p�s�ɂ���
		mStaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		mStaticMesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
		mStaticMesh->SetSimulatePhysics(true);	// �����V�~�����[�V������L���ɂ���
		mStaticMesh->RecreatePhysicsState();//�����G���W���ɑ΂��ẴR���|�[�l���g�̕�����Ԃ��č쐬
	}
	
}

//�L�����N�^�[�ƃI�[�o�[���b�v�i���������j
void AFallBlockActor::OverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	mTimeSwitch = true;
}

//�L�����N�^�[�ƃI�[�o�[���b�v�i���ꂽ���j
void AFallBlockActor::OverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	mTimeSwitch = false;
}



