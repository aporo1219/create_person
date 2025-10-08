// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CableComponent.h"
#include "TarzanRopeActor.generated.h"

UCLASS()
class MYPROJECT_API ATarzanRopeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATarzanRopeActor();

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UCableComponent* mCable;

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USceneComponent* mEndPoint;  //��ԓ��ł̈ʒu�E��]�E�g������I�u�W�F�N�g�i�K�w�\���E�A�^�b�`�����g�j
									 //����͂�����P�[�u���̖��[�p�R���|�[�l���g�Ƃ��Ĉ���

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UStaticMeshComponent* mMesh; //�X�^�e�B�b�N���b�V���R���|�[�l���g


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//�I�[�o�[���b�v
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool m_hit;

};
