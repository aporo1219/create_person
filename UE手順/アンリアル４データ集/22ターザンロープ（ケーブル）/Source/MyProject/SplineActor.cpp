// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineActor.h"
#include "MyPawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASplineActor::ASplineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // スプラインコンポーネントの作成と初期化
    mSpline = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
    RootComponent = mSpline;

    // 初期状態のスプラインポイントを追加
    mSpline->AddSplinePoint(FVector(0.0f, 0.0f, 0.0f), ESplineCoordinateSpace::Local, true);
    mSpline->AddSplinePoint(FVector(100.f, 0.0f, 0.0f), ESplineCoordinateSpace::Local, true);
    mSpline->UpdateSpline();  // スプラインの更新

     // インスタンス化されたメッシュコンポーネントの初期化
    mInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedMeshComponent"));
    mInstancedMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASplineActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    //ワールド上のポーン（キャラクター）の確認
    UWorld* const World = GetWorld();
    TArray<AActor*> pawn;
    UGameplayStatics::GetAllActorsOfClass(World, AMyPawn::StaticClass(), pawn);
    AMyPawn* p = Cast<AMyPawn>(pawn[0]);

    //スプラインとキャラクターのHITチェック
    float spline_pos=0.0f;
    bool  spline_hit=false;
    spline_hit=CheckIfCharacterHitSpline(p,spline_pos);

    //スプラインに沿ってキャラクターの強制移動
    if (spline_hit == true)
    {
        //少し前に進める
        spline_pos += 10.0f;
        // スプラインの長さを取得
        float SplineLength = mSpline->GetSplineLength();
        if (spline_pos > SplineLength)
        {
            ;
        }
        else
        {
            //スプラインの現在の位置の距離(spline_pos)から位置と回転を獲得
            FVector NewLocation = mSpline->GetLocationAtDistanceAlongSpline(spline_pos, ESplineCoordinateSpace::World);
            FRotator NewRotation = mSpline->GetRotationAtDistanceAlongSpline(spline_pos, ESplineCoordinateSpace::World);
            //キャラクター強制移動
            p->mCapsule->SetWorldLocation(NewLocation);
        }
    }



}

//スプラインとキャラクターのHITチェック
bool ASplineActor::CheckIfCharacterHitSpline(AMyPawn* Character, float& SplinePos)
{
    if (!Character || !mSpline)
    {
        return false;
    }

    // キャラクターの現在の位置を取得
    FVector CharacterLocation = Character->mCapsule->GetComponentLocation();
    // スプライン上の最も近い距離を取得
    float ClosestDistance = mSpline->FindInputKeyClosestToWorldLocation(CharacterLocation);
    // スプライン上の位置をワールド空間で取得
    FVector SplineClosestLocation = mSpline->GetLocationAtSplineInputKey(ClosestDistance, ESplineCoordinateSpace::World);
    // キャラクターの位置とスプライン上の最も近い点の間の距離を計算
    float DistanceToSpline = FVector::Dist(CharacterLocation, SplineClosestLocation);
    
    // スプラインに接触しているかどうかをチェック
    if (DistanceToSpline <= 100)
    {
        // 接触している場合、スプライン上の最も近い距離を返す
        float line = mSpline->GetDistanceAlongSplineAtSplineInputKey(ClosestDistance);
        UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("line=%f"), line), true, true, FColor::Cyan, 2.f);
        SplinePos = line;
        return true;
    }

    return false;
}

//エディタでのリアルタイム更新関数
void ASplineActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    if (!mInstancedMesh || !mSpline)
        return;

    // 既存のメッシュインスタンスをクリア
    mInstancedMesh->ClearInstances();

    // スプラインの長さを取得
    float SplineLength = mSpline->GetSplineLength();

    // スプラインに沿ってメッシュを配置
    for (float Distance = 0; Distance < SplineLength; Distance += 30)
    {
        // スプライン上の位置と方向を取得
        FVector  Location = mSpline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
        FRotator Rotation = mSpline->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
        Rotation.Add(-90, 0, 0);

        // メッシュインスタンスを追加
        FTransform InstanceTransform;
        InstanceTransform.SetLocation(Location);
        InstanceTransform.SetRotation(Rotation.Quaternion());
        mInstancedMesh->AddInstance(InstanceTransform);
    }
}
