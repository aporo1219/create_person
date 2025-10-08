// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProjectGameModeBase.h"
#include "MyPawn.h"
#include "DeathActor.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ADeathActor::ADeathActor()
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
	static ConstructorHelpers::FObjectFinder<UStaticMesh> VisualAsset(TEXT("/Engine/BasicShapes/Cone"));
	mStaticMesh->SetStaticMesh(VisualAsset.Object);//�o�^
	//�R���W�����ݒ�
	mStaticMesh->BodyInstance.SetCollisionProfileName("Projectile");//�R���W�����v���Z�b�g
	mStaticMesh->SetNotifyRigidBodyCollision(true);//�R���W������Hit�C�x���g���s�L���iSimulation Generates Hit Events�t���OON�j
	mStaticMesh->OnComponentHit.AddDynamic(this, &ADeathActor::OnHit);//�q�b�g�������̐U�镑�����郁�\�b�h�Z�b�g
}

// Called when the game starts or when spawned
void ADeathActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADeathActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ADeathActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//�N���C�}���f�[�^�̊m�F
	AMyPawn* p = Cast<AMyPawn>(OtherActor);
	if (p != nullptr)
	{
		if ( p->hit==false)
		{
			//�J�v�Z���ɑ΂���
			p->mCapsule->BodyInstance.SetCollisionProfileName("NoCollision");//�R���W�����Z�b�g
			p->mCapsule->SetSimulatePhysics(false);  // �����V�~�����[�V�����𖳌��ɂ���
			p->mCapsule->SetNotifyRigidBodyCollision(false);//�R���W��������
			p->mCapsule->RecreatePhysicsState();//�����G���W���ɑ΂��ẴR���|�[�l���g�̕�����Ԃ��č쐬

			//���b�V���ɑ΂���
			p->mSMesh->SetSimulatePhysics(true);  // �����V�~�����[�V������L���ɂ���
			p->mSMesh->SetNotifyRigidBodyCollision(true);
			p->mSMesh->BodyInstance.SetCollisionProfileName("BlockAllDynamic");//�R���W�����Z�b�g
			p->mSMesh->BodyInstance.SetInstanceNotifyRBCollision(true);
			p->mSMesh->BodyInstance.bLockXTranslation = true;//�R���X�g���C���g�@XY���s�ړ����b�N
			p->mSMesh->BodyInstance.bLockYTranslation = true;
			
			p->mSMesh->RecreatePhysicsState();//�����G���W���ɑ΂��ẴR���|�[�l���g�̕�����Ԃ��č쐬
			p->mSMesh->UpdateComponentToWorld();//�R���|�[�l���g���烏�[���h�ւ̕ϊ��̒l���Čv�Z

			//�x������
			UWorld* World = GetWorld();
			FTimerHandle _TimerHandle;
			World->GetTimerManager().SetTimer(_TimerHandle, this, &ADeathActor::HitGameOver, 3.0f,false);
			
						
			p->hit = true;
		}
	}

}

void ADeathActor::HitGameOver()
{
	AMyProjectGameModeBase* GameMode = Cast<AMyProjectGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->ReStart();//�Q�[�����[�h���̃��\�b�h���R�[��
	}
}