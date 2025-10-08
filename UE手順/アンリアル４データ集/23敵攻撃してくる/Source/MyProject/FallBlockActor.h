// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"			//ボックスコンポーネント
#include "FallBlockActor.generated.h"

UCLASS()
class MYPROJECT_API AFallBlockActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFallBlockActor();

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UStaticMeshComponent* mStaticMesh;//静的メッシュコンポーネント

	//オーバーラップ用
	UPROPERTY(Category = "C_CODE", VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* mBox;	//ボックスコンポーネント

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//落下用タイム
	int mTime{ 0 };
	//落下スイッチ
	bool mTimeSwitch{ false };
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//キャラクターとオーバーラップ（着いた時）
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//キャラクターとオーバーラップ（離れた時）
	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
