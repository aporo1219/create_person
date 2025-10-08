// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Particles/ParticleSystem.h"			//�p�[�e�B�N��
#include "Particles/ParticleSystemComponent.h"	//�p�[�e�B�N���R���|�[�l���g
#include "PhysicalMaterials/PhysicalMaterial.h" //�����}�e���A��
#include "Components/CapsuleComponent.h"		//�J�v�Z���R���|�[�l���g
#include "Components/BoxComponent.h"			//�{�b�N�X�R���|�[�l���g
#include "GameFramework/FloatingPawnMovement.h" //�t���[�����O�|�[���R���|�[�l���g
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "EnemyAIController.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "CoreMinimal.h"
#include "EnemyActor.generated.h"


//class UAIPerceptionComponent;
UCLASS()
class MYPROJECT_API AEnemyActor : public  APawn//���ύX�@AI�s���p���ߕύX
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyActor();

	//�v���p�e�B
	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USceneComponent* mRoot;			//���[�g�R���|�[�l���g�i�K�{�j
	UPROPERTY(Category = "C_CODE", VisibleAnywhere, BlueprintReadWrite)
		UCapsuleComponent* mCapsule;	//�J�v�Z���R���|�[�l���g
	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USkeletalMeshComponent* mSMesh;//�X�P���^�����b�V���R���|�[�l���g
	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UFloatingPawnMovement* mMove;	//���V�ړ��R���|�[�l���g

	//�ǉ����e�F���ɓ��񂾂甚��
	UPROPERTY(Category = "C_CODE", VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* mBox;	//�{�b�N�X�R���|�[�l���g

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UParticleSystemComponent* mParticle;//�p�[�e�B�N���V�X�e��

	//�ǉ����e�FAI�ɂ��ǔ�
	UPROPERTY(Category = "AI", VisibleAnywhere)
		UAIPerceptionComponent* mAIPerception;//AI���o�Z���T�[

	// ���o�ݒ�
	UPROPERTY(Category = "AI", VisibleAnywhere)
		UAISenseConfig_Sight* mSightConfig;//���o�ŔF��������p�����[�^��ݒ�p

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//�p���N�H�[�^�j�I��
	FQuat mNowQ;
	FQuat mNewQ;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	//�ǉ����e�F���ɓ��񂾂甚��
	UFUNCTION()
		void OnHitBox(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	//�ǉ����e�FAI�ɂ��ǔ�
	UFUNCTION()
		void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

};

