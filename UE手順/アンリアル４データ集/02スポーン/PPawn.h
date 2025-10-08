// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"	//カメラコンポーネント
#include "GameFramework/FloatingPawnMovement.h"//フローリングポーンコンポーネント
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PPawn.generated.h"



UCLASS()
class MYPROJECT_API APPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APPawn();
	//プロパティ
	UPROPERTY(Category = "C_CODE",VisibleAnywhere)
		USceneComponent* mRootCollider;	//ルートコンポーネント（必須）

	UPROPERTY(Category = "C_CODE",VisibleAnywhere)
		UStaticMeshComponent* mCubeMesh;//静的メッシュコンポーネント

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UCameraComponent* mCamera;		//カメラコンポーネント

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UFloatingPawnMovement* m_PawnMove;//浮遊移動コンポーネント

	
		
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	
	//コントローラーリアクションメソッド
	void Move_XAxis(float AxisValue);
	void Move_YAxis(float AxisValue);
	void Key_Attack();

};
