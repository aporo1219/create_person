// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PhysicalMaterials/PhysicalMaterial.h" //物理マテリアル
#include "GameFramework/SpringArmComponent.h"	//スプリングアームコンポーネント
#include "GameFramework/PlayerInput.h"			//プレイヤーインプット
#include "GameFramework/FloatingPawnMovement.h" //フローリングポーンコンポーネント
#include "Camera/CameraComponent.h"				//カメラコンポーネント
#include "Components/CapsuleComponent.h"		//カプセルコンポーネント
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
		USceneComponent* mRoot;			//ルートコンポーネント（必須）

	UPROPERTY(Category = "C_CODE", VisibleAnywhere, BlueprintReadWrite)
		UCapsuleComponent* mCapsule;	//カプセルコンポーネント

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USkeletalMeshComponent* mSMesh;//スケルタルメッシュコンポーネント

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USpringArmComponent* mSpringArm;//スプリングアームコンポーネント

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UCameraComponent* mCamera;		//カメラコンポーネント

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UFloatingPawnMovement* mMove;	//浮遊移動コンポーネント


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//移動キー方向ベクトル
	FVector mMoveVecX;
	FVector mMoveVecZ;

	//姿勢クォータニオン
	FQuat mNowQ;
	FQuat mNewQ;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//移動メソッド
	void Move_XAxis(float AxisValue);
	void Move_YAxis(float AxisValue);
	void Jump();

	//カメラ回転メソッド
	void CameraMove_XAxis(float Value);
	void CameraMove_YAxis(float Value);

	//衝突判定用
	bool hit;
};
