// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "BallPawn.h"
#include "MyProjectGameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"//PrintString

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//���[�g�R���|�[�l���g�쐬
	mRootCollider = CreateDefaultSubobject<USceneComponent>("RootCollider");
	RootComponent = Cast<USceneComponent>(mRootCollider);//���̃A�N�^�[��RootComponent�Ƃ��Đݒ�

	//�X�^�e�B�b�N���b�V���R���|�[�l���g�쐬
	mCubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//�X�^�e�B�b�N���b�V���u�L���[�u�r�W���A���v�̃A�Z�b�g�o�^
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Engine/BasicShapes/Cube"));
	mCubeMesh->SetStaticMesh(CubeVisualAsset.Object);//�o�^
	mCubeMesh->SetRelativeRotation(FRotator(30, 30, 0));//�e����̑��ΓI�ȉ�]�p�x
	mCubeMesh->SetRelativeLocation(FVector(0, 0, 0));	//�e����̑��ΓI�Ȉʒu

	mCubeMesh->BodyInstance.SetCollisionProfileName("OverlapAll");//�R���W�����v���Z�b�g(OverlapAll��ݒ�)
	mCubeMesh->SetGenerateOverlapEvents(true);//�I�[�o�[���b�v�C�x���g��true�ɂ���(GenerateOverlapEvents�t���OON)
	mCubeMesh->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OverlapBegin);//�I�[�o�[���b�v�X�^�[�g���C�x���g
	

	//��]���[�u�R���|�[�l���g�쐬
	mRotatMove = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatMove"));
	mRotatMove->UpdatedComponent=mCubeMesh;
	mRotatMove->bRotationInLocalSpace = true;		//��]�����[�J����ԂœK�p����邩�A���[���h��ԂœK�p����邩�B
	mRotatMove->PivotTranslation = FVector(0, 0, 0);//��]��ʒu
	mRotatMove->RotationRate = FRotator(0, 30, 0);	//��]���x

	
	//-----���[�g-----
	mCubeMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//�I�[�o�[���b�v�r�M�����\�b�h
void AItemActor::OverlapBegin( UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UKismetSystemLibrary::PrintString(this, "aaa", true, true, FColor::Cyan, 2.f);
	if (Cast<ABallPawn>(OtherActor) != nullptr)
	{
		mRotatMove->RotationRate = FRotator(0, 200, 0);	//��]���x

		//�x������(SetTimer���g����1�b���Collected�𓮂��悤�ɂ���)
		UWorld* const World = GetWorld();
		FTimerHandle _TimerHandle;
		World->GetTimerManager().SetTimer(_TimerHandle, this, &AItemActor::Collected, 1.0f, true);
	}
}
void AItemActor::Collected()
{
	//�F�؂��Ă�Q�[�����[�h���l��
	AMyProjectGameModeBase* GameMode = Cast<AMyProjectGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->ItemCollected();//�Q�[�����[�h���̃��\�b�h���R�[��
	}

	Destroy();//�A�N�^�[�폜
}


