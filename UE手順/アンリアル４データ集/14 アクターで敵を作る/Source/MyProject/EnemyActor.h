// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PhysicalMaterials/PhysicalMaterial.h" //物理マテリアル
#include "Components/CapsuleComponent.h"		//カプセルコンポーネント
#include "GameFramework/FloatingPawnMovement.h" //フローリングポーンコンポーネント
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

	//プロパティ
	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USceneComponent* mRoot;			//ルートコンポーネント（必須）
	UPROPERTY(Category = "C_CODE", VisibleAnywhere, BlueprintReadWrite)
		UCapsuleComponent* mCapsule;	//カプセルコンポーネント
	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USkeletalMeshComponent* mSMesh;//スケルタルメッシュコンポーネント
	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UFloatingPawnMovement* mMove;	//浮遊移動コンポーネント

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


};
