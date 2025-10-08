// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallActor.generated.h"

UCLASS()
class MYPROJECT_API ABallActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABallActor();

	//プロパティ
	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USceneComponent* mRootCollider;	//ルートコンポーネント（必須）

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UStaticMeshComponent* mCubeMesh;//静的メッシュコンポーネント

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
