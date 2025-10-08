// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/KismetSystemLibrary.h"//PrintString
#include "MyProjectGameModeBase.h"
#include "ItemActor.h"

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//USceneComponent* mRoot;//���[�g�R���|�[�l���g
	mRoot = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Cast<USceneComponent>(mRoot);//���̃A�N�^�[��RootComponent�Ƃ��Đݒ�

	//USkeletalMeshComponent* mSMesh;//�X�P���^�����b�V���R���|�[�l���g
	mSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SMesh"));
	mSMesh->SetupAttachment(mRoot);//RootComponent�̎q�ɂ���
	//�X�P���^�����b�V���o�^
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> VisualAsset(TEXT("/Game/Blade_ChickenBlade/SK_Blade_ChickenBlade.SK_Blade_ChickenBlade"));
	mSMesh->SetSkeletalMesh(VisualAsset.Object);//�o�^

	mSMesh->SetSimulatePhysics(true);	   //����ON
	mSMesh->SetGenerateOverlapEvents(true);//�I�[�o�[���b�v�C�x���g��true�ɂ���(GenerateOverlapEvents�t���OON)
	mSMesh->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OverlapBegin);//�I�[�o�[���b�v�X�^�[�g���C�x���g


	//�R���W�����Z�b�g �J�X�^���ݒ�
	mSMesh->BodyInstance.SetCollisionProfileName("Custom");
	mSMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);//�I�u�W�F�N�g�^�C�v�𓮂��Ă郂�m�ɂ���
	mSMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//�R���W�������X�|���X
	mSMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);//�S�ăI�[���u���b�N�ݒ�
	mSMesh->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);//�̂�Pawn���I�[�o�[���b�v�ݒ�

	
	


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
void AItemActor::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyProjectGameModeBase* GameMode = Cast<AMyProjectGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->mPoint += 100;
		Destroy();//�A�N�^�[�폜
	}

}

