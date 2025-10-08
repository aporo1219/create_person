// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineActor.h"
#include "MyPawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASplineActor::ASplineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // �X�v���C���R���|�[�l���g�̍쐬�Ə�����
    mSpline = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
    RootComponent = mSpline;

    // ������Ԃ̃X�v���C���|�C���g��ǉ�
    mSpline->AddSplinePoint(FVector(0.0f, 0.0f, 0.0f), ESplineCoordinateSpace::Local, true);
    mSpline->AddSplinePoint(FVector(100.f, 0.0f, 0.0f), ESplineCoordinateSpace::Local, true);
    mSpline->UpdateSpline();  // �X�v���C���̍X�V

     // �C���X�^���X�����ꂽ���b�V���R���|�[�l���g�̏�����
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

    //���[���h��̃|�[���i�L�����N�^�[�j�̊m�F
    UWorld* const World = GetWorld();
    TArray<AActor*> pawn;
    UGameplayStatics::GetAllActorsOfClass(World, AMyPawn::StaticClass(), pawn);
    AMyPawn* p = Cast<AMyPawn>(pawn[0]);

    //�X�v���C���ƃL�����N�^�[��HIT�`�F�b�N
    float spline_pos=0.0f;
    bool  spline_hit=false;
    spline_hit=CheckIfCharacterHitSpline(p,spline_pos);

    //�X�v���C���ɉ����ăL�����N�^�[�̋����ړ�
    if (spline_hit == true)
    {
        //�����O�ɐi�߂�
        spline_pos += 10.0f;
        // �X�v���C���̒������擾
        float SplineLength = mSpline->GetSplineLength();
        if (spline_pos > SplineLength)
        {
            ;
        }
        else
        {
            //�X�v���C���̌��݂̈ʒu�̋���(spline_pos)����ʒu�Ɖ�]���l��
            FVector NewLocation = mSpline->GetLocationAtDistanceAlongSpline(spline_pos, ESplineCoordinateSpace::World);
            FRotator NewRotation = mSpline->GetRotationAtDistanceAlongSpline(spline_pos, ESplineCoordinateSpace::World);
            //�L�����N�^�[�����ړ�
            p->mCapsule->SetWorldLocation(NewLocation);
        }
    }



}

//�X�v���C���ƃL�����N�^�[��HIT�`�F�b�N
bool ASplineActor::CheckIfCharacterHitSpline(AMyPawn* Character, float& SplinePos)
{
    if (!Character || !mSpline)
    {
        return false;
    }

    // �L�����N�^�[�̌��݂̈ʒu���擾
    FVector CharacterLocation = Character->mCapsule->GetComponentLocation();
    // �X�v���C����̍ł��߂��������擾
    float ClosestDistance = mSpline->FindInputKeyClosestToWorldLocation(CharacterLocation);
    // �X�v���C����̈ʒu�����[���h��ԂŎ擾
    FVector SplineClosestLocation = mSpline->GetLocationAtSplineInputKey(ClosestDistance, ESplineCoordinateSpace::World);
    // �L�����N�^�[�̈ʒu�ƃX�v���C����̍ł��߂��_�̊Ԃ̋������v�Z
    float DistanceToSpline = FVector::Dist(CharacterLocation, SplineClosestLocation);
    
    // �X�v���C���ɐڐG���Ă��邩�ǂ������`�F�b�N
    if (DistanceToSpline <= 100)
    {
        // �ڐG���Ă���ꍇ�A�X�v���C����̍ł��߂�������Ԃ�
        float line = mSpline->GetDistanceAlongSplineAtSplineInputKey(ClosestDistance);
        UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("line=%f"), line), true, true, FColor::Cyan, 2.f);
        SplinePos = line;
        return true;
    }

    return false;
}

//�G�f�B�^�ł̃��A���^�C���X�V�֐�
void ASplineActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    if (!mInstancedMesh || !mSpline)
        return;

    // �����̃��b�V���C���X�^���X���N���A
    mInstancedMesh->ClearInstances();

    // �X�v���C���̒������擾
    float SplineLength = mSpline->GetSplineLength();

    // �X�v���C���ɉ����ă��b�V����z�u
    for (float Distance = 0; Distance < SplineLength; Distance += 30)
    {
        // �X�v���C����̈ʒu�ƕ������擾
        FVector  Location = mSpline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
        FRotator Rotation = mSpline->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
        Rotation.Add(-90, 0, 0);

        // ���b�V���C���X�^���X��ǉ�
        FTransform InstanceTransform;
        InstanceTransform.SetLocation(Location);
        InstanceTransform.SetRotation(Rotation.Quaternion());
        mInstancedMesh->AddInstance(InstanceTransform);
    }
}
