// Fill out your copyright notice in the Description page of Project Settings.

#include "GlobalData.h"
#include "MySaveGame.h"
#include "GoalActor.h"
#include "GameFramework/PlayerStart.h"
#include "MyProjectGameModeBase.h"
#include "MyPawn.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGoalActor::AGoalActor()
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


	

}

// Called when the game starts or when spawned
void AGoalActor::BeginPlay()
{
	Super::BeginPlay();
	//�R���W����
	mStaticMesh->SetGenerateOverlapEvents(true);//�I�[�o�[���b�v�C�x���g��true�ɂ���(GenerateOverlapEvents�t���OON)
	mStaticMesh->BodyInstance.SetCollisionProfileName("OverlapAll");//�R���W�����v���Z�b�g(OverlapAll��ݒ�)
	mStaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AGoalActor::OverlapBegin);//�I�[�o�[���b�v�X�^�[�g���C�x���g


	
	//��24�Z�[�u�ƃ��[�h�A�����L���O�ł̒ǉ�
	//GameInstance�̃O���[�o���f�[�^�̃A�h���X�l��
	AMyProjectGameModeBase* GameMode = Cast<AMyProjectGameModeBase>(GetWorld()->GetAuthGameMode());
	UGlobalData* instance = Cast<UGlobalData>(GetGameInstance());
	//�t�@�C�����uScoreGameData�v����Z�[�u�f�[�^�����[�h����
	UMySaveGame* saveData =Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot("ScoreGameData", 0));
	if (saveData != nullptr)
	{
		//���[�h�����f�[�^���O���[�o���f�[�^�ɓ����
		int32* pp = instance->GetData();
		for (int i = 0; i < 5; i++)
			pp[i] = saveData->Score[i];

		saveData->MarkPendingKill();
	}
}

// Called every frame
void AGoalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//�I�[�o�[���b�v�r�M�����\�b�h
void AGoalActor::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	//�N���C�}���f�[�^�̊m�F�i�����_�Ŏg�p����ׁA�A�h���X�������Ă�������p�̓����o�j
	p = Cast<AMyPawn>(OtherActor);
	if (p != nullptr)
	{
		//��24�Z�[�u�ƃ��[�h�A�����L���O�ł̒ǉ�
		//GameInstance�̃O���[�o���f�[�^�Ƀf�[�^��o�^
		AMyProjectGameModeBase* GameMode = Cast<AMyProjectGameModeBase>(GetWorld()->GetAuthGameMode());
		UGlobalData* instance = Cast<UGlobalData>(GetGameInstance());
		
		if (GameMode && instance)
		{
			instance->SetData(GameMode->mPoint);
		}

		//
		// UMySaveGame�̃C���X�^���X�𐶐�.
		UMySaveGame* saveData = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
		ensureMsgf(saveData, TEXT("saveData is null."));

		if (saveData!=nullptr)
		{
			int32* pp=instance->GetData();
			for (int i = 0; i < 5; i++)
				saveData->Score[i] = pp[i];
			//�f�[�^�Z�[�u
			UGameplayStatics::SaveGameToSlot(saveData, saveData->SlotNameGameData, sizeof(int32)*5);
			saveData->MarkPendingKill();
		}
	
		

		//�S�[���F��
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Goal")), true, true, FColor::Cyan, 2.f);


		// PlayerStart �I�u�W�F�N�g�������� (PlayerStartTag�� "Respawn" ����)
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
		
		AActor* ChosenPlayerStart = nullptr;
		for (AActor* PlayerStart : PlayerStarts)
		{
			APlayerStart* ps = Cast<APlayerStart>(PlayerStart);
			if (ps != nullptr)
			{
				if (ps->PlayerStartTag == FName("Respawn"))
				{
					ChosenPlayerStart = PlayerStart;
					break;
				}
			}
		}
		
	
		// �ʒu��ݒ�
		p->mCapsule->SetWorldLocation(ChosenPlayerStart->GetActorLocation(),false,nullptr, ETeleportType::ResetPhysics);
	
		//�ʒu�ύX�ɂ��ړ��͔�����}�����邽�߈ꎞ�I�ɓ����Ȃ��悤�ɂ���
		p->mCapsule->BodyInstance.bLockXTranslation = true;
		p->mCapsule->BodyInstance.bLockYTranslation = true;
		p->mCapsule->BodyInstance.bLockZTranslation = true;
		p->mCapsule->RecreatePhysicsState();

		//�x������(���߂̓����_�ŋL��)
		UWorld*  World = GetWorld();
		FTimerHandle _TimerHandle;
		World->GetTimerManager().SetTimer(_TimerHandle, [&]()
			{
				//�ʒu�ύX�ɂ��ړ��͔����̓^�C�~���O���Ⴄ�̂ŁA�x�������Ĉړ��͂�0�ɂ���
				p->mMove->Velocity = FVector::ZeroVector;
				p->mCapsule->BodyInstance.bLockXTranslation = false;
				p->mCapsule->BodyInstance.bLockYTranslation = false;
				p->mCapsule->BodyInstance.bLockZTranslation = false;
				p->mCapsule->RecreatePhysicsState();
			},
		0.1f, false);

	
	}


}