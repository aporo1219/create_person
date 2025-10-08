// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PhysicalMaterials/PhysicalMaterial.h" //�����}�e���A��
#include "Components/CapsuleComponent.h"		//�J�v�Z���R���|�[�l���g
#include "GameFramework/FloatingPawnMovement.h" //�t���[�����O�|�[���R���|�[�l���g
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyActor.generated.h"

UCLASS()
class MYPROJECT_API AEnemyActor : public AActor
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


};
