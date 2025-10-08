// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Particles/ParticleSystem.h"			//パーティクル
#include "Particles/ParticleSystemComponent.h"	//パーティクルコンポーネント
#include "PhysicalMaterials/PhysicalMaterial.h" //物理マテリアル
#include "Components/CapsuleComponent.h"		//カプセルコンポーネント
#include "Components/BoxComponent.h"			//ボックスコンポーネント
#include "GameFramework/FloatingPawnMovement.h" //フローリングポーンコンポーネント
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "EnemyAIController.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "CoreMinimal.h"
#include "EnemyActor.generated.h"


//class UAIPerceptionComponent;
UCLASS()
class MYPROJECT_API AEnemyActor : public  APawn//※変更　AI行動用ため変更
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

	//追加内容：頭に踏んだら爆発
	UPROPERTY(Category = "C_CODE", VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* mBox;	//ボックスコンポーネント

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UParticleSystemComponent* mParticle;//パーティクルシステム

	//追加内容：AIによる追尾
	UPROPERTY(Category = "AI", VisibleAnywhere)
		UAIPerceptionComponent* mAIPerception;//AI感覚センサー

	// 視覚設定
	UPROPERTY(Category = "AI", VisibleAnywhere)
		UAISenseConfig_Sight* mSightConfig;//視覚で認識条件やパラメータを設定用

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//姿勢クォータニオン
	FQuat mNowQ;
	FQuat mNewQ;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	//追加内容：頭に踏んだら爆発
	UFUNCTION()
		void OnHitBox(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	//追加内容：AIによる追尾
	UFUNCTION()
		void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

};

