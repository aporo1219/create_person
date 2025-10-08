// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "MyPawn.h"
#include "SplineActor.generated.h"

UCLASS()
class MYPROJECT_API ASplineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineActor();

	// スプラインコンポーネントをエディタ上で編集可能にする
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spline")
		USplineComponent* mSpline;

	// インスタンス化されたメッシュコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UInstancedStaticMeshComponent* mInstancedMesh;


	//エディタでのリアルタイム更新関数
	virtual void OnConstruction(const FTransform& Transform) override;


	bool CheckIfCharacterHitSpline(AMyPawn* Character,float& SplinePos);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
