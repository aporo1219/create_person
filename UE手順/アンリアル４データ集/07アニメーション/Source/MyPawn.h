// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"	//�J�����R���|�[�l���g
#include "GameFramework/FloatingPawnMovement.h"//�t���[�����O�|�[���R���|�[�l���g
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

UCLASS()
class MYPROJECT_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

	//�v���p�e�B
	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USceneComponent* mRootCollider;	//���[�g�R���|�[�l���g�i�K�{�j

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USkeletalMeshComponent* mSMesh;//�X�P���^�����b�V���R���|�[�l���g
		
	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UCameraComponent* mCamera;		//�J�����R���|�[�l���g

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UFloatingPawnMovement* m_PawnMove;//���V�ړ��R���|�[�l���g


	//���s���x 
	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		float Speed = 0.0f;
	//���s�X�C�b�`
	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		bool IsWalk =false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move_XAxis(float AxisValue);
	void Move_YAxis(float AxisValue);
};
