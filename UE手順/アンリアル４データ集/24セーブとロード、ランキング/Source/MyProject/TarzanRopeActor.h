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
		USceneComponent* mEndPoint;  //空間内での位置・回転・拡大を持つオブジェクト（階層構造・アタッチメント）
									 //今回はこれをケーブルの末端用コンポーネントとして扱う

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UStaticMeshComponent* mMesh; //スタティックメッシュコンポーネント


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//オーバーラップ
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool m_hit;

};
