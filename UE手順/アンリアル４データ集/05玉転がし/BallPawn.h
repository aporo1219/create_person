// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SpringArmComponent.h"	//�X�v�����O�A�[���R���|�[�l���g
#include "Camera/CameraComponent.h"				//�J�����R���|�[�l���g
#include "GameFramework/PlayerInput.h"			//�v���C���[�C���v�b�g
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BallPawn.generated.h"



UCLASS()
class MYPROJECT_API ABallPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABallPawn();
	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USceneComponent* mRootCollider;	//���[�g�R���|�[�l���g�i�K�{�j

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UStaticMeshComponent* mCubeMesh;//�ÓI���b�V���R���|�[�l���g

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USpringArmComponent * mSpringArm;//�X�v�����O�A�[���R���|�[�l���g

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UCameraComponent* mCamera;		//�J�����R���|�[�l���g


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	float mMoveForce;
	int   mJumpCount;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void MoveRight(float value);
	void MoveForward(float value);
	void Jump();
};
