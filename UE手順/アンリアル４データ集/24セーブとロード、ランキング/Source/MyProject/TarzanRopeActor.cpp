// Fill out your copyright notice in the Description page of Project Settings.


#include "TarzanRopeActor.h"
#include "Kismet/GameplayStatics.h"
#include "MyPawn.h"

// Sets default values
ATarzanRopeActor::ATarzanRopeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // �P�[�u���R���|�[�l���g���쐬���A���[�g�R���|�[�l���g�ɐݒ�
    mCable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
    RootComponent = mCable;


    // �P�[�u���̖��[�p�V�[���R���|�[�l���g���쐬
    mEndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("EndPoint"));
    mEndPoint->SetWorldLocation(FVector(0,0,0));


    // �P�[�u���̐ݒ�
    //�P�[�u���̃G���h����ݒ��EndSocket�ɐݒ�A�P�[�u���G���h��mEndPoint�œ�����
    mCable->SetAttachEndToComponent(mEndPoint, FName("EndSocket"));
    mCable->CableLength = 300.0f; // �P�[�u���̒�����ݒ�
    mCable->bAttachStart = true;  //�P�[�u���̐�[���A�^�b�`���āA�ʒu���Œ�
    mCable->bAttachEnd   = false;   
    mCable->bEnableCollision = false;//�P�[�u���̃Z�O�����g�ɏՓ˔����OFF
  

    //���f���A�Z�b�g�o�^
    static ConstructorHelpers::FObjectFinder<UStaticMesh> VisualAsset(TEXT("/Engine/BasicShapes/Cube"));
    //�X�^�e�B�b�N���b�V���R���|�[�l���g�쐬
    mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    mMesh->SetupAttachment(mCable);
    mMesh->SetStaticMesh(VisualAsset.Object);//�o�^
    // �����̐ݒ�
    mMesh->SetRelativeScale3D(FVector(0.5)); // �T�C�Y�𒲐�
    mMesh->SetRelativeLocation(FVector(0, 0, 0)); // �ʒu�𒲐�
    mMesh->BodyInstance.SetCollisionProfileName("OverlapAll");//�R���W�����Z�b�g
    mMesh->SetSimulatePhysics(false);  // �����V�~�����[�V�����𖳌��ɂ���
    mMesh->SetNotifyRigidBodyCollision(false);
    mMesh->SetGenerateOverlapEvents(true);
    mMesh->OnComponentBeginOverlap.AddDynamic(this, &ATarzanRopeActor::OverlapBegin);
    mMesh->OnComponentEndOverlap.AddDynamic(this, &ATarzanRopeActor::OverlapEnd);
}

// Called when the game starts or when spawned
void ATarzanRopeActor::BeginPlay()
{
	Super::BeginPlay();

    m_hit = false;
}

// Called every frame
void ATarzanRopeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    //�P�[�u���̐��i�p�[�e�B�N���j�̈ʒu���擾
    TArray<FVector> Locations;
    mCable->GetCableParticleLocations(Locations);
 
  
    //�擪�E���[�l��
    FVector  end_pos   = Locations[Locations.Num() - 1];//���[
    FVector  endf_pos  = Locations[Locations.Num() - 2];//���[�̑O
    FVector  start_pos = Locations[0];                  //�擪
    mMesh->SetWorldLocation(end_pos);//mEndPoint��擪�ʒu�ɐݒ�


    //���[�Ɛ擪�̈ʒu��������x�N�g�����擾
    FQuat mNowQ;
    FVector w(0, 0, 1);
    FVector vec_cable = endf_pos - end_pos;
    vec_cable.Normalize();
    //�����x�N�g������N�H�[�^�j�I�����쐬
    float r = FVector::DotProduct(w, vec_cable);		//w�x�N�g����0�x�Ƃ��ĉ�]�p�x
    FVector rvec = FVector::CrossProduct(w, vec_cable);	//w�x�N�g���ƈړ����������]���x�N�g�����߂�
    rvec.Normalize();//��]���x�N�g���𐳋K��
    //�@���̏ꍇ�A�s�x�N�g������������ꍇ������
    if (rvec.IsNearlyZero())
        rvec = FVector(0, 0, 1);
    //��]�p�x�Ɖ�]���x�N�g������N�H�[�^�j�I�����擾
    mNowQ = FQuat(rvec, acos(r));

    //mEndPoint�����������ɉ�]
    mMesh->SetWorldRotation(mNowQ);

    if (m_hit == true)
    {
        //�L�����N�^�[�ɃP�[�u���̖��[����������
        //���[���h���̃A�N�^�[��T��
        UWorld* const World = GetWorld();
        TArray<AActor*> a;
        UGameplayStatics::GetAllActorsOfClass(World, AMyPawn::StaticClass(), a);
        if (a[0] != nullptr)
        {
            AMyPawn* p = Cast<AMyPawn>(a[0]);

            //�L�����N�^�[�����ړ�
            p->mCapsule->SetWorldLocation(end_pos);
            p->mSMesh->SetWorldRotation(mNowQ);

            //���[�v����̉���
            if (p->go == true)
            {
                m_hit = false;
                p->mCapsule->SetSimulatePhysics(true); 
                p->mCapsule->SetNotifyRigidBodyCollision(true);//�R���W�����L��
                p->mCapsule->RecreatePhysicsState();           //������Ԃ��č쐬
            }
        }
    }
   
}


//�L�����N�^�[�ƃI�[�o�[���b�v�i���������j
void ATarzanRopeActor::OverlapBegin(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
   

    AMyPawn* p = Cast<AMyPawn>(&OtherActor[0]);
    if (p != nullptr)
    {
        UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("hit_in")), true, true, FColor::Cyan, 2.f);
        m_hit = true;


        p->go = false;
        //�P�[�u���Ɉړ������̗͂�������
        FVector vec = p->mMove->Velocity;
        mCable->CableForce.Set(vec.X, vec.Y, vec.Z);
        mCable->CableForce.Normalize();
        mCable->CableForce.X *= 3000;
        mCable->CableForce.Y *= 3000;
        mCable->CableForce.Z *= 3000;
       
        //�x������
        UWorld* World = GetWorld();
        FTimerHandle _TimerHandle;
        World->GetTimerManager().SetTimer(_TimerHandle, [&]()
            {
                mCable->CableForce.Set(0, 0, 0);//�����o���Ă���A�͂�0�ɂ��Ď��R�ɂ���
            },
            0.5f, false);

        p->mCapsule->SetSimulatePhysics(false);
        p->mCapsule->SetNotifyRigidBodyCollision(false);//�R���W��������
        p->mCapsule->RecreatePhysicsState();            //������Ԃ��č쐬
    }

  
}

//�L�����N�^�[�ƃI�[�o�[���b�v�i���ꂽ���j
void ATarzanRopeActor::OverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("hit_out")), true, true, FColor::Cyan, 2.f);
    m_hit = false;

    AMyPawn* p = Cast<AMyPawn>(&OtherActor[0]);
    if (p != nullptr)
    {
        p->mCapsule->SetSimulatePhysics(true);
        p->mCapsule->SetNotifyRigidBodyCollision(true);//�R���W�����L��
        p->mCapsule->RecreatePhysicsState();           //������Ԃ��č쐬
    }
}

