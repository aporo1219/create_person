// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Kismet/KismetSystemLibrary.h"//PrintString
// Sets default values
AMyPawn::AMyPawn()
{
	//�|�[���̃X�|�[�����APlayerController �������I�Ƀ|�[�������L���邩������iPlayer0��ݒ�j
	AutoPossessPlayer = EAutoReceiveInput::Player0;

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//USceneComponent* mRoot;//���[�g�R���|�[�l���g
	mRoot = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Cast<USceneComponent>(mRoot);//���̃A�N�^�[��RootComponent�Ƃ��Đݒ�

	//UCapsuleComponent* mCapsule;	//�J�v�Z���R���|�[�l���g
	mCapsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	mCapsule->SetupAttachment(RootComponent);
	mCapsule->SetSimulatePhysics(true);					   //����ON
	mCapsule->BodyInstance.SetCollisionProfileName("Pawn");//�R���W�����Z�b�g
	//�R���X�g���C���g�@��]���b�N
	mCapsule->BodyInstance.bLockXRotation = true;
	mCapsule->BodyInstance.bLockYRotation = true;
	mCapsule->BodyInstance.bLockZRotation = true;

	//�����}�e���A���A�Z�b�g��T��
	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PMatAsset(TEXT("/Game/NewPhysicalMaterial.NewPhysicalMaterial"));
	if (PMatAsset.Succeeded())
	{
		mCapsule->SetPhysMaterialOverride(PMatAsset.Object);//�J�v�Z���R���|�[�l���g�ɕ����}�e���A����ݒ�
	}
	
	

	//USkeletalMeshComponent* mSMesh;//�X�P���^�����b�V���R���|�[�l���g
	mSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SMesh"));
	mSMesh->SetupAttachment(mCapsule);//RootComponent�̎q�ɂ���
	//�X�P���^�����b�V���o�^
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> VisualAsset(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));
	mSMesh->SetSkeletalMesh(VisualAsset.Object);//�o�^
	//���t�@�����X����X�|�[������N���X���擾
	FString path = "/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C";
	TSubclassOf<class UAnimInstance> sclass;//TSubclassOf�́AClass�^�̈��S����񋟂���e���v���[�g�N���X
	sclass = TSoftClassPtr<UAnimInstance>(FSoftObjectPath(*path)).LoadSynchronous();
	mSMesh->SetAnimClass((UClass*)sclass);

	

	//USpringArmComponent* mSpringArm �X�v�����O�A�[���R���|�[�l���g
	mSpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	mSpringArm->TargetArmLength = 400.0f;
	mSpringArm->bInheritPitch = false;//Pitch�ERoll�EYaw���I�t���ăJ������]�𖳂���
	mSpringArm->bInheritYaw = false;
	mSpringArm->bInheritRoll = false;
	mSpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	mSpringArm->SetupAttachment(mSMesh);

	//UCameraComponent* mCamera; �J�����R���|�[�l���g�쐬
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mCamera->SetupAttachment(mSpringArm);

	//UFloatingPawnMovement* mMove;	//���V�ړ��R���|�[�l���g
	mMove = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Move"));
	mMove->UpdatedComponent = mCapsule;//�ړ���X�V���邷��R���|�[�l���g��ݒ�
	
	
	
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	//�ړ��L�[�����x�N�g���̏�����
	mMoveVecX = FVector(0, 0, 0);
	mMoveVecZ = FVector(0, 0, 0);
	//�p���N�H�[�^�j�I���̏�����
	mNowQ = FQuat(FVector(0, 0, 1), 0.0f);
	mNewQ = FQuat(FVector(0, 0, 1), 0.0f);
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�ړ�����
	FVector mMoveVec= mMoveVecX + mMoveVecZ;
	mMoveVec.Normalize();

	//�ړ�
	mMove->AddInputVector(mMoveVec, false);

	//�V�����p�����l��
	if (mMoveVec.Size() > 0.1f)
	{
		FVector w(0, 1, 0);
		float r = FVector::DotProduct(w, mMoveVec);			//w�x�N�g����0�x�Ƃ��ĉ�]�p�x
		FVector rvec = FVector::CrossProduct(w,mMoveVec);	///w�x�N�g���ƈړ����������]���x�N�g�����߂�
		rvec.Normalize();//��]���x�N�g���𐳋K��

		//�@���̏ꍇ�A�s�x�N�g������������ꍇ������
		if (rvec.IsNearlyZero())
			rvec = FVector(0, 0, 1);

		//��]�p�x�Ɖ�]���x�N�g������N�H�[�^�j�I�����擾
		mNewQ = FQuat(rvec,acos(r));
	}
	
	//���݂̎p���ƐV�����p������
	mNowQ = FQuat::Slerp(mNowQ, mNewQ, 0.1f);

	//�p�������ɉ�]������
	mSMesh->SetRelativeRotation(mNowQ);

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//�f�t�H���g�R���g���[�����̃}�b�s���O
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

//�ړ����\�b�h
void AMyPawn::Move_XAxis(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		FVector vec(1.0f, 1.0f, 0.0);
		mMoveVecX = mCamera->GetRightVector()*vec;//Z���j��
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
		mMoveVecZ = mCamera->GetForwardVector() * vec;//Z���j��
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

//�J������]���\�b�h
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