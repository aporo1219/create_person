// Copyright Epic Games, Inc. All Rights Reserved.


#include "MyProjectGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.h"
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

	//NewWidgetBlueprint�̍쐬
	path = "/Game/NewWidgetBlueprint.NewWidgetBlueprint_C";
	TSubclassOf<class UUserWidget> GameWigetClass;
	GameWigetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*path)).LoadSynchronous();
	if (GameWigetClass)
	{
		GameWidget = CreateWidget<UMyUserWidget>(GetWorld(), GameWigetClass);
		if (GameWidget)
		{
			//�r���[�|�[�g�`��
			GameWidget->AddToViewport();
			UpdateItemText();
		}

	}

}

void AMyProjectGameModeBase::UpdateItemText()
{
	GameWidget->SetItemText(ItemsCollected, ItemsInLevel);//�E�B�W�F�b�g�̃f�[�^�X�V
}

void AMyProjectGameModeBase::ItemCollected()
{
	ItemsCollected++;
	UpdateItemText();
}
