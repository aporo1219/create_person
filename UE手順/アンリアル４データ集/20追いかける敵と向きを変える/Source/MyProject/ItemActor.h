// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

UCLASS()
class MYPROJECT_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

	//�v���p�e�B
	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USceneComponent* mRoot;			//���[�g�R���|�[�l���g�i�K�{�j
	UPROPERTY(Category = "C_CODE", VisibleAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* mSMesh;//�X�P���^�����b�V���R���|�[�l���g
	
	//�R���W�����I�[�o�[���b�v
	UFUNCTION()
		void OverlapBegin(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
