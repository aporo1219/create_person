// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyActor.h"
#include "MyPawn.h"
#include "EnemyAIController.h"
#include "MyProjectGameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionSystem.h"
#include "GameFramework/Pawn.h"


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


	//�ǉ����e�FAI�ɂ��ǔ�
	mAIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	
	// ���o�ݒ�
	mSightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	mSightConfig->SightRadius = 2000.0f;  // ���o�͈̔�
	mSightConfig->LoseSightRadius = 2200.0f;  // �������͈�
	mSightConfig->PeripheralVisionAngleDegrees = 60.0f;  // ����p

	mSightConfig->DetectionByAffiliation.bDetectEnemies = true;
	mSightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	mSightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	
	mSightConfig->AutoSuccessRangeFromLastSeenLocation = 1;//�ꎞ�I�Ɍ������Ă��͈͓��ł���Ί��m����
	mSightConfig->SetMaxAge(0.1f);//���m��Y�����܂ł̎���
	
	mAIPerception->ConfigureSense(*mSightConfig);//���o�̃Z�b�g
	mAIPerception->SetDominantSense(UAISense_Sight::StaticClass());//���o�̗D��N���X�ݒ�
	
	// ���o�C�x���g�̐ݒ�
	mAIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyActor::OnPerceptionUpdated);
	

	
	//�ǉ����e�FAI�ɂ��ǔ�
	// AIController��ݒ肷��
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;  // �Q�[�����ŃX�|�[�������Ƃ���AIController�������ŃA�^�b�`
	AIControllerClass = AEnemyAIController::StaticClass();   // AIController�̃N���X���w��
}

// Called when the game starts or when spawned
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();
	
	mSMesh->SetRelativeLocation(FVector(0, 0, -40));//�X�P���^�����b�V���̏����I�t�Z�b�g�ʒu
	mBox->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));//�R���W����BOX�̏����I�t�Z�b�g�ʒu

	
	
	
	//SpawnDefaultController();//�o�^���Ă�AIController�𐶐�

	 // ���x�̐ݒ�
	if (mMove)
	{
		mMove->MaxSpeed     = 200.0f;    // �ő呬�x
		mMove->Acceleration = 2048.0f;  // �����x
		mMove->Deceleration = 2048.0f;  // �����x
	}

	//�p���N�H�[�^�j�I���̏�����
	mNowQ = FQuat(FVector(0, 0, 1), 0.0f);
	mNewQ = FQuat(FVector(0, 0, 1), 0.0f);

}

// Called every frame
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�ǉ����e�FAI�ɂ��ǔ�
	FVector wpos;
	FRotator wrot;
	wrot = mSMesh->GetRelativeRotation();
	wpos = mCapsule->GetRelativeLocation();
	mRoot->SetRelativeRotation(wrot+FRotator(0,90,0));
	mRoot->SetRelativeLocation(wpos);

	//�ړ������Ƀ��f���̌�����ς���
	FVector mMoveVec = mMove->Velocity;//�t���[�����O���[�u�R���e���g����ړ��x�N�g�����l��
	mMoveVec.Z = 0.0f;//XY���x�N�g���ɂ���
	mMoveVec.Normalize();

	//�V�����p�����l���iMyPawn����R�s�y�j
	if (mMoveVec.Size() > 0.1f)
	{
		FVector w(0, 1, 0);
		float r = FVector::DotProduct(w, mMoveVec);			//w�x�N�g����0�x�Ƃ��ĉ�]�p�x
		FVector rvec = FVector::CrossProduct(w, mMoveVec);	///w�x�N�g���ƈړ����������]���x�N�g�����߂�
		rvec.Normalize();//��]���x�N�g���𐳋K��

		//�@���̏ꍇ�A�s�x�N�g������������ꍇ������
		if (rvec.IsNearlyZero())
			rvec = FVector(0, 0, 1);

		//��]�p�x�Ɖ�]���x�N�g������N�H�[�^�j�I�����擾
		mNewQ = FQuat(rvec, acos(r));
	}

	//���݂̎p���ƐV�����p������
	mNowQ = FQuat::Slerp(mNowQ, mNewQ, 0.1f);

	//�p�������ɉ�]������(�@�Փˎ��F���f���𕨗��V�~�����[�V��������̂Ńe���|�[�g�t���O��ON�ɂ���@)
	mSMesh->SetRelativeRotation(mNowQ, false, nullptr, ETeleportType::ResetPhysics);
	
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
	
	if (HitDirection <= -0.5)
	{
		//�R���|�[�l���g�̃p�[�e�B�N����Ɨ����Ď��s
		UParticleSystem* ParticleTemplate = mParticle->Template;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleTemplate, GetActorLocation(), GetActorRotation());
		Destroy();//�A�N�^�[�폜
	}
}

//�ǉ����e�FAI�ɂ��ǔ�
void AEnemyActor::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{ 
	
	if (Stimulus.WasSuccessfullySensed())
	{
		// ���o��Actor�𔭌��������̏���
		//���o�Ō������v���C���[
		AMyPawn* player = Cast<AMyPawn>(&Actor[0]);
		//AI�R���g���[���[�̊m�F
		AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
		
		// AIController�Ƀv���C���[����ݒ�
		if (AIController && player)
		{
			AIController->SetPlayerKey(player);
		}
	}
	else
	{
		// ���o����O�ꂽ���̏���
	}
	
}

