// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h" //物理マテリアル
#include "GameFramework/Actor.h"
#include "MoveBlockActor.generated.h"

UCLASS()
class MYPROJECT_API AMoveBlockActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMoveBlockActor();

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USceneComponent* mRoot;//ルートコンポーネント
	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UStaticMeshComponent* mStaticMesh;//静的メッシュコンポーネント

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float mMoveTime;
	FVector mLocation;


	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
