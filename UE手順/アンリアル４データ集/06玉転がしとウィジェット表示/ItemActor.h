// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/RotatingMovementComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"


UCLASS()
class MYPROJECT_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USceneComponent* mRootCollider;	//ルートコンポーネント（必須）
	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UStaticMeshComponent* mCubeMesh;//静的メッシュコンポーネント
	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		URotatingMovementComponent* mRotatMove;//回転ムーブコンポーネント
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OverlapBegin( UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void Collected();

};
