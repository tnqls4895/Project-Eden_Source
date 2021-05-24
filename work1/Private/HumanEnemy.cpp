#include "HumanEnemy.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EdenGameMode.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/EngineTypes.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

AHumanEnemy::AHumanEnemy()
{
	SearchPointX = CreateDefaultSubobject<USceneComponent>(TEXT("SearchPointX"));
	SearchPointY = CreateDefaultSubobject<USceneComponent>(TEXT("SearchPointY"));

	

	PipeBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("PipeBoxComponent"));
	PipeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PipeStaticMeshComponent"));

	PipeMeshComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Socket_Weapon"));
	PipeBoxComponent->AttachToComponent(PipeMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AHumanEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetDetectingSound();
	SetView();
	SetAttackCircle();
	ChangeState();
	DoState();

	UE_LOG(LogClass, Log, TEXT("Current Aggro %f"),Aggression);

	if (IsInView() == true)
	{
		ViewAggressionUp();
	}
	GetCharacterMovement()->MaxWalkSpeed = Speed;

	if (OnlySearch)
	{
		if (CurState == EEnemyState::Search || CurState == EEnemyState::Watch)
		{
			Aggression = 11;
		}
	}
}

void AHumanEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitLocation = GetActorLocation();
	MyViewAggro = 5;

}

//

void AHumanEnemy::SetDetectingSound()
{
	//ĸ���� ������ ������Ʈ�� ���� array�� �����
	TArray<TEnumAsByte<EObjectTypeQuery> > TargetObjects;
	//pawn�� ����ǵ��� �Ѵ�(collision�������� pawn�� 3��° ��ġ������)
	TargetObjects.Add(EObjectTypeQuery::ObjectTypeQuery3);
	TArray<AActor*> ActorToIgnore;
	TArray<AActor*> OutActors;

	//�Ҹ� ���� ���� ����
	bool bIsSuccessCapsuleOverlapActors =
		UKismetSystemLibrary::CapsuleOverlapActors(GetWorld(), GetActorLocation(),
			RadiusDetectSound, RadiusDetectSound, TargetObjects, nullptr, ActorToIgnore, OutActors);

	//������ �Ǿ�����
	if (bIsSuccessCapsuleOverlapActors)
	{
		//����� ���͵鸮��Ʈ����
		for (auto Object : OutActors)
		{
			//���� ĳ���͸� ã�´�.
			APawn* ControlledCharacter = Cast<APawn>(Object);//���� ĳ���͸� ������ ����
			//������ ���� ���ͷ� �Ѿ��
			if (ControlledCharacter == nullptr)
			{
				continue;
			}
			//������
			else
			{
				//���� �÷��̾ �����ϰ� �ִ� pawn�� ���� ĳ�������� Ȯ���ϰ�
				if (GetWorld()->GetFirstPlayerController()->GetPawn() == ControlledCharacter)
				{
					//���� ������ ���Դٰ� üũ
					bIsInSoundCircle = true;
				}
			}
		}
	}
}

void AHumanEnemy::SetView()
{
	//�⺻�� false
	bIsInView = false;

	FVector CurPlayerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector MyPos = GetActorLocation();

	CurPlayerPos.Z = 0;
	MyPos.Z = 0;

	int Distance = (CurPlayerPos - MyPos).Size();
	if (Distance <= RadiusView)
	{
		//���� ���
		FVector A = (CurPlayerPos - MyPos).GetSafeNormal();
		A.Z = 0;
		FVector B = GetActorForwardVector();
		B.Z = 0;
		float DotA = FVector::DotProduct(A, B);
		float Angle = FMath::Acos(DotA) * 180/PI;
		if (Angle <= AngleView)
		{
			//���𰡿� ������ ������ ������ �ȵǵ��� �ϴ� LineTrace ����
			UWorld* pWorld = GetWorld();
			if (pWorld != nullptr)
			{
				FVector Start = GetActorLocation();
				FVector End = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

				FHitResult HitResult = FHitResult();
				FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
				Params.AddIgnoredActor(this);

				pWorld->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel3, Params);

				if (HitResult.Actor != nullptr)
				{
					//Hit �� Actor�� �÷��̾���
					if (HitResult.Actor == GetWorld()->GetFirstPlayerController()->GetPawn())
					{
						bIsInView = true;
					}
				}
			}
		}
	}
}

void AHumanEnemy::SetAttackCircle()
{
	bIsInAttack = false;
	//ĸ���� ������ ������Ʈ�� ���� array�� �����
	TArray<TEnumAsByte<EObjectTypeQuery> > TargetObjects;
	//pawn�� ����ǵ��� �Ѵ�(collision�������� pawn�� 3��° ��ġ������)
	TargetObjects.Add(EObjectTypeQuery::ObjectTypeQuery3);
	TArray<AActor*> ActorToIgnore;
	TArray<AActor*> OutActors;

	//�Ҹ� ���� ���� ����
	bool bIsSuccessCapsuleOverlapActors =
		UKismetSystemLibrary::CapsuleOverlapActors(GetWorld(), GetActorLocation(),
			RadiusAttack, RadiusAttack, TargetObjects, nullptr, ActorToIgnore, OutActors);

	//������ �Ǿ�����
	if (bIsSuccessCapsuleOverlapActors)
	{
		//����� ���͵鸮��Ʈ����
		for (auto Object : OutActors)
		{
			//���� ĳ���͸� ã�´�.
			APawn* ControlledCharacter = Cast<APawn>(Object);//���� ĳ���͸� ������ ����
			//������ ���� ���ͷ� �Ѿ��
			if (ControlledCharacter == nullptr)
			{
				continue;
			}
			//������
			else
			{
				//���� �÷��̾ �����ϰ� �ִ� pawn�� ���� ĳ�������� Ȯ���ϰ�
				if (GetWorld()->GetFirstPlayerController()->GetPawn() == ControlledCharacter)
				{
					//���� ������ ���Դٰ� üũ
					bIsInAttack = true;
				}
			}
		}
	}
}

bool AHumanEnemy::IsInSoundCircle()
{
	return bIsInSoundCircle;
}

bool AHumanEnemy::IsInView()
{
	return bIsInView;
}

//

/*
void AHumanEnemy::SetLocationAPlayer()
{
	//�÷��̾� ��ġ ����
	LocationAPlayer = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
}

FVector AHumanEnemy::GetLocationAPlayer()
{
	return LocationAPlayer;
}
*/

void AHumanEnemy::ChangeState()
{
	EEnemyState CheckState;
	CheckState = CheckConditionState();

	SetState(CheckState);
}

EEnemyState AHumanEnemy::CheckConditionState()
{
	if (PlayBeAttackedAnim)
	{
		if (CurState == EEnemyState::Watch || CurState == EEnemyState::Search)
		{
			Aggression = 40;
			return EEnemyState::BeAttacked;
		}
	}

	//Watch
	//��׷� ��ġ�� �ٸ� ���·� ������ �� �ִ� ��ġ�� �ƴ϶��
	if (Aggression < 10)
	{
		//Watch�� ����
		return EEnemyState::Watch;
	}
	/*
	else if ((Aggression >= 10 && Aggression < 40) && bIsInView == false)
	{
		return EEnemyState::Search;
	}
	*/
	//Search
	//��׷� ��ġ�� 10�̻� 40�̸��϶�(���), ���� ���°� �����
	else if ((Aggression >= 10 && Aggression < 40) && CurState == EEnemyState::Watch)
	{
		//�þ߿� �÷��̾ ���ٸ�
		if (bIsInView == false)
		{
			//Search�� ����(Watch -> Search)
			return EEnemyState::Search;
		}
	}
	//��׷� ��ġ�� 10�̻� 40�̸��϶�(�϶�), ���� ���°� �����̶��
	else if ((Aggression >= 10 && Aggression < 40) && CurState == EEnemyState::Track)
	{
		//�þ߿� �÷��̾ ���ٸ�
		if (bIsInView == false)
		{
			//Search�� ����(Track -> Search)
			return EEnemyState::Search;
		}
	}

	//Track
	//��׷� ��ġ�� 40�̻� 50�̸��϶�(���), ���� ���°� �����̶��
	else if ((Aggression >= 40 && Aggression <= 50) && CurState == EEnemyState::Search)
	{
		//�������·� ����
		return EEnemyState::Track;
	}
	//���� ���°� ���������̰� �þ߿� �÷��̾ �ִٸ�
	else if (CurState == EEnemyState::Search && bIsInView == true)
	{
		Aggression = 40;
		//�������·� ����
		return EEnemyState::Track;
	}
	//�����¿��� ��׷ΰ� 10 �̻��̰� �þ߿� �÷��̾ �ִٸ�
	else if (Aggression < 10 && CurState == EEnemyState::Watch)
	{
		if (bIsInView == true)
		{
			Aggression = 40;
			return EEnemyState::Track;
		}
	}
	//�ǰݽ� �������� �����ϴ� �ڵ� ¥��

	
	//Attack
	//��� ���¿��� ���ݰ��ɹ��� �ȿ� �÷��̾ �ִٸ� ����
	else if (bIsInAttack == true)
	{
		return EEnemyState::Attack;
	}
	else if (bIsInAttack == false && bIsPlayingAttackAnim == false)
	{
		if (CurState == EEnemyState::Attack)
		{
			return EEnemyState::Track;
		}
	}


	return CurState;
}

void AHumanEnemy::SetState(EEnemyState NewState)
{
	//���� ���¿� NewState�� �ٸ��ٸ�(���ٸ� ���� �ٲ��ʿ� ����)
	if (CurState != NewState)
	{
		//���� ���¸� ������
		LeaveState(CurState);
		//NewState ����
		EnterNewState(NewState);

		CurState = NewState;

		if (EnemySpeed.Contains(CurState))
		{
			Speed = EnemySpeed[CurState];
		}
	}
}

void AHumanEnemy::LeaveState(EEnemyState LeaveCurState)
{
	//LeaveCurState�� ����
	if (LeaveCurState == EEnemyState::Watch)
	{
		//�׿� �´� �ൿ
	}

	//LeaveCurState�� �����̸�
	if (LeaveCurState == EEnemyState::Search)
	{
		//�׿� �´� �ൿ
	}

	//LeaveCurState�� �����̸�
	if (LeaveCurState == EEnemyState::Track)
	{
		//�׿� �´� �ൿ
	}

	//LeaveCurState�� �����̸�
	if (LeaveCurState == EEnemyState::Attack)
	{
		//�׿� �´� �ൿ
	}
}

void AHumanEnemy::EnterNewState(EEnemyState EnterNewState)
{
	//EnterNewState�� ����
	if (EnterNewState == EEnemyState::Watch)
	{
		//�׿� �´� �ൿ(�Լ��� ������!)
		EnterWatch();
	}

	//EnterNewState�� �����̸�
	if (EnterNewState == EEnemyState::Search)
	{
		//�׿� �´� �ൿ(�Լ��� ������!)
		EnterSearch();
	}

	//EnterNewState�� �����̸�
	if (EnterNewState == EEnemyState::Track)
	{
		//�׿� �´� �ൿ(�Լ��� ������!)
		EnterTrack();
	}

	//EnterNewState�� �����̸�
	if (EnterNewState == EEnemyState::Attack)
	{
		//�׿� �´� �ൿ(�Լ��� ������!)
		EnterAttack();
	}
}

//�������Խ� �ൿ
void AHumanEnemy::EnterWatch()
{
	if (EnemySpeed.Contains(CurState))
	{
		Speed = EnemySpeed[CurState];
	}
	//���� ���� ����
}

//�� ����� ���� Ȯ���ϱ�
void AHumanEnemy::EnterSearch()
{
	bIsSearchX = false;
	bIsVisitX = false;
	bIsVisitY = false;
	WaitTime = 0.0f;
	SearchCount = 0;

	Search_PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	//Y�� �� ������
	if (FVector::Dist(GetActorLocation(), SearchPointX->GetComponentLocation()) > 
		FVector::Dist(GetActorLocation(), SearchPointY->GetComponentLocation()))
	{
		bIsSearchX = false;
	}
	else
	{
		bIsSearchX = true;
	}
}

void AHumanEnemy::EnterTrack()
{
	if (EnemySpeed.Contains(CurState))
	{
		Speed = EnemySpeed[CurState];
	}
	//�����Ҷ� ��׷� ��ġ�� 40���� ����
	Aggression = 40;
}

void AHumanEnemy::EnterAttack()
{
	AttackCount = 0;
	bCanAttack = true;
}

/*
void AHumanEnemy::EnterBeAttacked()
{
	Aggression = 40;
}
*/

//���º� �ൿ
void AHumanEnemy::DoWatch()
{
	//�ൿ
	//HUD��ȭ

	//��׷� �϶�
	//��׷ΰ� 0�� �ƴϸ�
	if (Aggression != 0)
	{
		//��׷� ����
		AggressionDown();
	}
}
void AHumanEnemy::DoSearch()
{
	//UE_LOG(LogTemp, Log, TEXT("DoSearch"));
	//X�� �� ������� X���� �湮
	if (bIsSearchX == true)
	{
		if (SearchCount == 10)
		{
			AAIController* AIController = Cast<AAIController>(GetController());
			if (AIController)
			{
				//���ʽ����������� �̵�
				Cast<AAIController>(GetController())->MoveToLocation(InitLocation);
			}

			//�����ϸ� ��׷� ���� ����
			if (GetActorLocation() == InitLocation || OnlySearch == false)
			{
				Aggression = 9;
			}
			else if (GetActorLocation() == InitLocation || OnlySearch == true)
			{
				SearchCount = 0;
			}
		}
		
		else if (SearchCount % 2 == 0)
		{
			//X�� �̵�
			//�ü� ������ �Ѿư��� ����
			GetCharacterMovement()->bOrientRotationToMovement = true;

			AAIController* AIController = Cast<AAIController>(GetController());
			if (FVector::Dist(GetActorLocation(), SearchPointX->GetComponentLocation()) < 100)
			{
				float Timer = GetWorld()->GetDeltaSeconds();
				if (WaitTime <= 1)
				{
					WaitTime += Timer;
				}
				else
				{
					SearchCount++;
					WaitTime = 0;
				}
			}
			
			else
			{
				if (AIController)
				{
					//X�� �̵�
					Cast<AAIController>(GetController())->MoveToLocation(SearchPointX->GetComponentLocation());
				}
			}
		}
		else
		{
			//Y�� �̵�
			//�ü� ������ �Ѿư��� ����
			GetCharacterMovement()->bOrientRotationToMovement = true;

			if (FVector::Dist(GetActorLocation(), SearchPointY->GetComponentLocation()) < 100)
			{
				float Timer = GetWorld()->GetDeltaSeconds();
				if (WaitTime <= 1)
				{
					WaitTime += Timer;
				}
				else
				{
					SearchCount++;
					WaitTime = 0;
				}
			}
			else
			{
				AAIController* AIController = Cast<AAIController>(GetController());
				if (AIController)
				{
					//Y�� �̵�
					Cast<AAIController>(GetController())->MoveToLocation(SearchPointY->GetComponentLocation());
				}
			}

		}
	}
	//Y�� �� ������� Y���� �湮
	else
	{
		if (SearchCount == 10)
		{
			AAIController* AIController = Cast<AAIController>(GetController());
			if (AIController)
			{
				//���ʽ����������� �̵�
				Cast<AAIController>(GetController())->MoveToLocation(InitLocation);
			}

			//�����ϸ� ��׷� ���� ����
			if (GetActorLocation() == InitLocation || OnlySearch == false)
			{
				Aggression = 9;
			}
			else if (GetActorLocation() == InitLocation || OnlySearch == true)
			{
				SearchCount = 0;
			}
		}

		if (SearchCount % 2 == 0)
		{
			//Y�� �̵�
			//�ü� ������ �Ѿư��� ����
			GetCharacterMovement()->bOrientRotationToMovement = true;

			if (FVector::Dist(GetActorLocation(), SearchPointY->GetComponentLocation()) < 100)
			{
				float Timer = GetWorld()->GetDeltaSeconds();
				if (WaitTime <= 1)
				{
					WaitTime += Timer;
				}
				else
				{
					SearchCount++;
					WaitTime = 0;
				}
			}
			else
			{
				AAIController* AIController = Cast<AAIController>(GetController());
				if (AIController)
				{
					//Y�� �̵�
					Cast<AAIController>(GetController())->MoveToLocation(SearchPointY->GetComponentLocation());
				}
			}
		}
		else
		{
			//X�� �̵�
			//�ü� ������ �Ѿư��� ����
			GetCharacterMovement()->bOrientRotationToMovement = true;

			AAIController* AIController = Cast<AAIController>(GetController());
			if (FVector::Dist(GetActorLocation(), SearchPointX->GetComponentLocation()) < 100)
			{
				float Timer = GetWorld()->GetDeltaSeconds();
				if (WaitTime <= 1)
				{
					WaitTime += Timer;
				}
				else
				{
					SearchCount++;
					WaitTime = 0;
				}
			}

			else
			{
				if (AIController)
				{
					//X�� �̵�
					Cast<AAIController>(GetController())->MoveToLocation(SearchPointX->GetComponentLocation());
				}
			}
		}
	}
}

void AHumanEnemy::DoTrack()
{
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	AAIController* AIController = Cast<AAIController>(GetController());

	//�ü������� �÷��̾���ġ
	//�ü� ������ ������
	GetCharacterMovement()->bOrientRotationToMovement = false;

	FRotator EnemyRotator = GetActorRotation();
	FRotator ChangeRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerLocation);
	SetActorRotation(FRotator(EnemyRotator.Pitch, ChangeRotator.Yaw, EnemyRotator.Roll));

	if (AIController)
	{
		//�÷��̾� �Ѿư�
		Cast<AAIController>(GetController())->MoveToLocation(PlayerLocation);
	}

	//HUD��ȭ

	//�÷��̾ �þ߰��� ���ٸ�
	if (bIsInView == false)
	{
		//�ʴ� 10�� ��׷� ����
		AggressionDown();
	}
}

void AHumanEnemy::DoAttack()
{
	//�ü��� �÷��̾�� ����
	//�ִ�3�� ���� ����
	
}

void AHumanEnemy::DoState()
{
	switch (CurState)
	{
	case EEnemyState::Watch:
		DoWatch();
		break;
	case EEnemyState::Search:
		DoSearch();
		break;
	case EEnemyState::Track:
		DoTrack();
		break;
	case EEnemyState::Attack:
		DoAttack();
		break;
	}
}

void AHumanEnemy::OnDamage(float Damage)
{
	PlayBeAttackedAnim = true;

	bIsPlayingAttackAnim = false;
	PipeBoxComponent->SetGenerateOverlapEvents(false);

	HP = HP - Damage;
	if (GetHP() <= 0)
	{
		HP = 0;
		//�׾����� ó��
		OnDeath();
		Killed = true;
	}
}

float AHumanEnemy::GetHP()
{
	return HP;
}

void AHumanEnemy::ViewAggressionUp()
{
	//�ʴ� 5����
	AggressionUp(MyViewAggro * GetWorld()->DeltaTimeSeconds);
}

void AHumanEnemy::HideThings(USceneCaptureComponent2D* SceneComponent)
{
	SceneComponent->HideComponent(GetMesh());
}

void AHumanEnemy::OnStartAttack()
{
	bIsPlayingAttackAnim = true;
	PipeBoxComponent->SetGenerateOverlapEvents(true);
}

void AHumanEnemy::OnEndAttack()
{
	bIsPlayingAttackAnim = false;
	PipeBoxComponent->SetGenerateOverlapEvents(false);
}

void AHumanEnemy::OnEndAttacked()
{
	PlayBeAttackedAnim = false;
}

bool AHumanEnemy::GetIsAttacking()
{
	return bCanAttack && bIsInAttack;
}

void AHumanEnemy::OnDeath()
{
	Super::OnDeath();
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}

void AHumanEnemy::StealthKill()
{
	bCanStealthKill = true;
}