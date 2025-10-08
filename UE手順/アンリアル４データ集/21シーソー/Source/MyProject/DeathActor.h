// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeathActor.generated.h"

UCLASS()
class MYPROJECT_API ADeathActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USceneComponent* mRoot;//ルートコンポーネント
	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UStaticMeshComponent* mStaticMesh;//静的メッシュコンポーネント
	
public:	
	// Sets default values for this actor's properties
	ADeathActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

	void HitGameOver();


	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
