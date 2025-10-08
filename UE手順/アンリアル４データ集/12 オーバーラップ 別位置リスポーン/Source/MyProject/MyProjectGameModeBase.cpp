// Copyright Epic Games, Inc. All Rights Reserved.

#include "Kismet/KismetSystemLibrary.h"//PrintString
#include "Kismet/GameplayStatics.h"
#include "MyPawn.h"
#include "MyProjectGameModeBase.h"

void AMyProjectGameModeBase::BeginPlay()
{
	
}

//���X�|�[���@���x���I�[�v��
void AMyProjectGameModeBase::ReStart()
{
	//���x���ăI�[�v��
	UWorld* World = GetWorld();
	if (World)
	{ 
		UGameplayStatics::OpenLevel(World, FName(*World->GetName()), false);
	}
}

//���X�|�[�� �@�v���C���[�X�^�[�g�ʒu����̃��X�^�[�g
void AMyProjectGameModeBase::RespawnPlayerStart(AController* Controller, AActor* PlayerStart)
{
	if (Controller && PlayerStart)
	{	
		//����̃|�[����j��
		APawn* OldPawn = Controller->GetPawn();
		OldPawn->Destroy();

		//�X�|�[�������郊�t�@�����X
		FString path = "/Script/MyProject.MyPawn";
		//���t�@�����X����X�|�[������N���X���擾
		TSubclassOf<class AMyPawn> spaw_class;//TSubclassOf�́AClass�^�̈��S����񋟂���e���v���[�g�N���X
		spaw_class = TSoftClassPtr<AActor>(FSoftObjectPath(*path)).LoadSynchronous();

		// ���[���h�ɃA�N�^�[���X�|�[��
		APawn* NewPawn = GetWorld()->SpawnActor<AMyPawn>(spaw_class, PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation());
		Controller->Possess(NewPawn);
		
	}
}