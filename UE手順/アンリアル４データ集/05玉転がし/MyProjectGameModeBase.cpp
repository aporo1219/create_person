// Copyright Epic Games, Inc. All Rights Reserved.


#include "MyProjectGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "ItemActor.h"
#include "BallPawn.h"

AMyProjectGameModeBase::AMyProjectGameModeBase()
{ 
	//�f�t�H���g�|�[���N���X��ABallPawn�ɐݒ�
	DefaultPawnClass = ABallPawn::StaticClass();
}

void AMyProjectGameModeBase::BeginPlay()
{
	//�X�|�[�������郊�t�@�����X
	FString path = "/Script/MyProject.ItemActor";
	//���t�@�����X����X�|�[������N���X���擾
	TSubclassOf<class AItemActor> spaw_class;//TSubclassOf�́AClass�^�̈��S����񋟂���e���v���[�g�N���X
	spaw_class = TSoftClassPtr<AActor>(FSoftObjectPath(*path)).LoadSynchronous();

	// ���[���h�ɃA�N�^�[���X�|�[��
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		AItemActor* p;
		p= World->SpawnActor<AItemActor>(spaw_class);
		p->mCubeMesh->SetRelativeLocation(FVector(0, 0, 70));

		p = World->SpawnActor<AItemActor>(spaw_class);
		p->mCubeMesh->SetRelativeLocation(FVector(250, 0, 75));

		p = World->SpawnActor<AItemActor>(spaw_class);
		p->mCubeMesh->SetRelativeLocation(FVector(0, 250, 80));

		p = World->SpawnActor<AItemActor>(spaw_class);
		p->mCubeMesh->SetRelativeLocation(FVector(250, 250, 85));
	}

	//���[���h����AItem�A�N�^�[��T��
	TArray<AActor* > Imtes;
	UGameplayStatics::GetAllActorsOfClass(World, AItemActor::StaticClass(), Imtes);
	ItemsInLevel = Imtes.Num();
}

void AMyProjectGameModeBase::UpdateItemText()
{

}

void AMyProjectGameModeBase::ItemCollected()
{
	ItemsCollected++;
}
