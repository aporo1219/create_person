// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"	//カメラコンポーネント
#include "GameFramework/FloatingPawnMovement.h"//フローリングポーンコンポーネント
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

	//プロパティ
	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USceneComponent* mRootCollider;	//ルートコンポーネント（必須）

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USkeletalMeshComponent* mSMesh;//スケルタルメッシュコンポーネント
		
	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UCameraComponent* mCamera;		//カメラコンポーネント

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UFloatingPawnMovement* m_PawnMove;//浮遊移動コンポーネント


	//歩行速度 
	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		float Speed = 0.0f;
	//歩行スイッチ
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
