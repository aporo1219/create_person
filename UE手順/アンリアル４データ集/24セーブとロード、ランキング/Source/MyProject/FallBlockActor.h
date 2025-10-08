// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"			//�{�b�N�X�R���|�[�l���g
#include "FallBlockActor.generated.h"

UCLASS()
class MYPROJECT_API AFallBlockActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFallBlockActor();

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UStaticMeshComponent* mStaticMesh;//�ÓI���b�V���R���|�[�l���g

	//�I�[�o�[���b�v�p
	UPROPERTY(Category = "C_CODE", VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* mBox;	//�{�b�N�X�R���|�[�l���g

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//�����p�^�C��
	int mTime{ 0 };
	//�����X�C�b�`
	bool mTimeSwitch{ false };
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//�L�����N�^�[�ƃI�[�o�[���b�v�i���������j
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//�L�����N�^�[�ƃI�[�o�[���b�v�i���ꂽ���j
	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
