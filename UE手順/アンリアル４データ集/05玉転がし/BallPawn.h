// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SpringArmComponent.h"	//スプリングアームコンポーネント
#include "Camera/CameraComponent.h"				//カメラコンポーネント
#include "GameFramework/PlayerInput.h"			//プレイヤーインプット
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
		USceneComponent* mRootCollider;	//ルートコンポーネント（必須）

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UStaticMeshComponent* mCubeMesh;//静的メッシュコンポーネント

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USpringArmComponent * mSpringArm;//スプリングアームコンポーネント

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UCameraComponent* mCamera;		//カメラコンポーネント


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
