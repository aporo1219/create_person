// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PhysicalMaterials/PhysicalMaterial.h" //�����}�e���A��
#include "GameFramework/SpringArmComponent.h"	//�X�v�����O�A�[���R���|�[�l���g
#include "GameFramework/PlayerInput.h"			//�v���C���[�C���v�b�g
#include "GameFramework/FloatingPawnMovement.h" //�t���[�����O�|�[���R���|�[�l���g
#include "Camera/CameraComponent.h"				//�J�����R���|�[�l���g
#include "Components/CapsuleComponent.h"		//�J�v�Z���R���|�[�l���g
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
		USceneComponent* mRoot;			//���[�g�R���|�[�l���g�i�K�{�j

	UPROPERTY(Category = "C_CODE", VisibleAnywhere, BlueprintReadWrite)
		UCapsuleComponent* mCapsule;	//�J�v�Z���R���|�[�l���g

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USkeletalMeshComponent* mSMesh;//�X�P���^�����b�V���R���|�[�l���g

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USpringArmComponent* mSpringArm;//�X�v�����O�A�[���R���|�[�l���g

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UCameraComponent* mCamera;		//�J�����R���|�[�l���g

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UFloatingPawnMovement* mMove;	//���V�ړ��R���|�[�l���g


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//�ړ��L�[�����x�N�g��
	FVector mMoveVecX;
	FVector mMoveVecZ;

	//�p���N�H�[�^�j�I��
	FQuat mNowQ;
	FQuat mNewQ;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//�ړ����\�b�h
	void Move_XAxis(float AxisValue);
	void Move_YAxis(float AxisValue);
	void Jump();

	//�J������]���\�b�h
	void CameraMove_XAxis(float Value);
	void CameraMove_YAxis(float Value);

	//�Փ˔���p
	bool hit;
};
