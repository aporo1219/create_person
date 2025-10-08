// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "MyProjectGameModeBase.h"
#include "EnemyActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyActor::AEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//USceneComponent* mRoot;//���[�g�R���|�[�l���g
	mRoot = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Cast<USceneComponent>(mRoot);//���̃A�N�^�[��RootComponent�Ƃ��Đݒ�

	//UCapsuleComponent* mCapsule;	//�J�v�Z���R���|�[�l���g
	mCapsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	mCapsule->SetupAttachment(RootComponent);
	mCapsule->SetSimulatePhysics(true);					   //����ON
	mCapsule->BodyInstance.SetCollisionProfileName("Pawn");//�R���W�����Z�b�g
	mCapsule->SetNotifyRigidBodyCollision(true);//�R���W������Hit�C�x���g���s�L���iSimulation Generates Hit Events�t���OON�j
	mCapsule->OnComponentHit.AddDynamic(this, &AEnemyActor::OnHit);//�q�b�g�������̐U�镑�����郁�\�b�h�Z�b�g
	//�R���X�g���C���g�@��]���b�N
	mCapsule->BodyInstance.bLockXRotation = true;
	mCapsule->BodyInstance.bLockYRotation = true;
	mCapsule->BodyInstance.bLockZRotation = true;

	
	//USkeletalMeshComponent* mSMesh;//�X�P���^�����b�V���R���|�[�l���g
	mSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SMesh"));
	mSMesh->SetupAttachment(mCapsule);//RootComponent�̎q�ɂ���
	//�X�P���^�����b�V���o�^
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> VisualAsset(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin_Female.SK_Mannequin_Female"));
	mSMesh->SetSkeletalMesh(VisualAsset.Object);//�o�^
	//���t�@�����X����X�|�[������N���X���擾
	FString path = "/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C";
	TSubclassOf<class UAnimInstance> sclass;//TSubclassOf�́AClass�^�̈��S����񋟂���e���v���[�g�N���X
	sclass = TSoftClassPtr<UAnimInstance>(FSoftObjectPath(*path)).LoadSynchronous();
	mSMesh->SetAnimClass((UClass*)sclass);

	//UFloatingPawnMovement* mMove;	//���V�ړ��R���|�[�l���g
	mMove = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Move"));
	mMove->UpdatedComponent = mCapsule;//�ړ���X�V���邷��R���|�[�l���g��ݒ�

	//�ǉ����e�F���ɓ��񂾂甚��
	//�ǉ�
	mParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	mParticle->SetupAttachment(mSMesh);
	mParticle->bAutoActivate = false;// �����A�N�e�B�u���𖳌��ɂ��邱������Ȃ��ƁA�o�^���Ɏ��s�����
	
	//�@�p�[�e�B�N���o�^
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/FXVarietyPack/Particles/P_ky_hit2.P_ky_hit2"));
	if (ParticleAsset.Succeeded())
	{
		mParticle->SetTemplate(ParticleAsset.Object); 
	}


	//UBoxComponent* mBox
	mBox = CreateDefaultSubobject<UBoxComponent>("Box");
	mBox->SetupAttachment(mCapsule);
	//mBox->SetSimulatePhysics(true);					   //����ON
	mBox->BodyInstance.SetCollisionProfileName("Pawn");//�R���W�����Z�b�g
	mBox->SetNotifyRigidBodyCollision(true);//�R���W������Hit�C�x���g���s�L���iSimulation Generates Hit Events�t���OON�j
	mBox->OnComponentHit.AddDynamic(this, &AEnemyActor::OnHitBox);//�q�b�g�������̐U�镑�����郁�\�b�h�Z�b�g
	
	//�Փ˔���`��f�o�b�N�p
	mBox->SetHiddenInGame(false);  //�Q�[�����ɉB���Ȃ�
	mBox->SetVisibility(true);     //��������
	mBox->ShapeColor = FColor::Red;// �f�o�b�O�p�̐F��Ԃɐݒ�

	mCapsule->SetHiddenInGame(false);
	mCapsule->SetVisibility(true);
	mCapsule->ShapeColor = FColor::Red;
}

// Called when the game starts or when spawned
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();
	mSMesh->SetRelativeLocation(FVector(0, 0, -40));//�X�P���^�����b�V���̏����I�t�Z�b�g�ʒu
	mBox->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));//�R���W����BOX�̏����I�t�Z�b�g�ʒu

}

// Called every frame
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//�N���C�}���f�[�^�̊m�F
	AMyPawn* p = Cast<AMyPawn>(OtherActor);
	if (p != nullptr)
	{
		if (p->hit == false)
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
			World->GetTimerManager().SetTimer(_TimerHandle, [&]()
			{
				//�Q�[�����[�h���̃��\�b�h���R�[��
				AMyProjectGameModeBase* GameMode = Cast<AMyProjectGameModeBase>(GetWorld()->GetAuthGameMode());
				if (GameMode)
					GameMode->ReStart();
			}, 3.0f, false);


			p->hit = true;
		}
	}

}

//�ǉ����e�F���ɓ��񂾂甚��
void AEnemyActor::OnHitBox(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//�Փ˂����ʂ̕�����Z���̎�
	float HitDirection = Hit.Normal.Z;

	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("t=%f"), HitDirection), true, true, FColor::Cyan, 2.f);


	if (HitDirection <= -0.5)
	{
		// �R���|�[�l���g�̃p�[�e�B�N���V�X�e�������s
		//mParticle->ActivateSystem(); 
		//UKismetSystemLibrary::PrintString(this, TEXT("HIT"), true, true, FColor::Cyan, 2.f);

		//�R���|�[�l���g�̃p�[�e�B�N����Ɨ����Ď��s
		UParticleSystem* ParticleTemplate = mParticle->Template;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleTemplate, GetActorLocation(), GetActorRotation());
		Destroy();//�A�N�^�[�폜
	}
}

