// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "MyPawn.h"
#include "SplineActor.generated.h"

UCLASS()
class MYPROJECT_API ASplineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineActor();

	// �X�v���C���R���|�[�l���g���G�f�B�^��ŕҏW�\�ɂ���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spline")
		USplineComponent* mSpline;

	// �C���X�^���X�����ꂽ���b�V���R���|�[�l���g
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UInstancedStaticMeshComponent* mInstancedMesh;


	//�G�f�B�^�ł̃��A���^�C���X�V�֐�
	virtual void OnConstruction(const FTransform& Transform) override;


	bool CheckIfCharacterHitSpline(AMyPawn* Character,float& SplinePos);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
