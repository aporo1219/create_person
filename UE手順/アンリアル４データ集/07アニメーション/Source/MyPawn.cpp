// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Animation/AnimBlueprint.h"
#include "GameFramework/PlayerInput.h"
#include "MyAnimInstance.h"

// Sets default values
AMyPawn::AMyPawn()
{
	//�|�[���̃X�|�[�����APlayerController �������I�Ƀ|�[�������L���邩������iPlayer0��ݒ�j
	AutoPossessPlayer = EAutoReceiveInput::Player0;

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//���[�g�R���|�[�l���g�쐬
	mRootCollider = CreateDefaultSubobject<USceneComponent>("RootCollider");
	RootComponent = Cast<USceneComponent>(mRootCollider);//���̃A�N�^�[��RootComponent�Ƃ��Đݒ�

	//�X�P���^�����b�V���R���|�[�l���g�쐬
	mSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SMesh"));
	mSMesh->SetupAttachment(RootComponent);//RootComponent�̎q�ɂ���
	
										   
	//�X�P���^�����b�V���o�^
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> VisualAsset(TEXT("/Game/XBot.XBot"));
	mSMesh->SetSkeletalMesh(VisualAsset.Object);//�o�^

	//���t�@�����X����X�|�[������N���X���擾
	FString path = "/Game/NewAnimBlueprint.NewAnimBlueprint_C";
	TSubclassOf<class UMyAnimInstance> sclass;//TSubclassOf�́AClass�^�̈��S����񋟂���e���v���[�g�N���X
	sclass = TSoftClassPtr<UMyAnimInstance>(FSoftObjectPath(*path)).LoadSynchronous();
	mSMesh->SetAnimInstanceClass(sclass);//�A�j���C���X�^���X�o�^


	//mSMesh->SetSimulatePhysics(true);  // �����V�~�����[�V������L���ɂ���
	//mSMesh->BodyInstance.SetCollisionProfileName("Pawn");//�R���W�����Z�b�g

	//�J�����R���|�[�l���g�쐬
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mCamera->SetupAttachment(mSMesh);//mCubeMesh�̎q�ɂȂ�
	mCamera->SetRelativeLocation(FVector(-200.0f, 0.0f, 200.0f));//(x,y,z����)�@�J�����ʒu
	mCamera->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));//�J�����̉�]�p�x

	//�t���[�����O�|�[�����[�u�R���|�[�l���g�쐬
	m_PawnMove = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMove"));
	m_PawnMove->UpdatedComponent = mSMesh;//�ړ���X�V���邷��R���|�[�l���g��ݒ�
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	//�f�t�H���g�R���g���[�����̃}�b�s���O
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MOVEX", EKeys::A, -1));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MOVEX", EKeys::D, 1));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MOVEY", EKeys::W, 1));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MOVEY", EKeys::S, -1));
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MOVEX", this, &AMyPawn::Move_XAxis);
	InputComponent->BindAxis("MOVEY", this, &AMyPawn::Move_YAxis);
}

//�ړ����\�b�h
void AMyPawn::Move_XAxis(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		m_PawnMove->AddInputVector(GetActorRightVector() * AxisValue, false);//ActorRightVector�����Ɉړ�
		IsWalk = true;
		Speed += 5.0f;
		if (Speed >= 100.0f)
			Speed = 100.0f;
	}

	Speed -= 2.0f;
	if (Speed < 0.0f)
	{
		Speed = 0.0f;
		IsWalk = false;
	}
	

}
void AMyPawn::Move_YAxis(float AxisValue)
{

	if (AxisValue != 0.0f)
	{
		m_PawnMove->AddInputVector(GetActorForwardVector() * AxisValue, false);//ActorForwardVector�����Ɉړ�
	}
}