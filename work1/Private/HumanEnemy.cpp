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
	//캡슐로 검출할 오브젝트를 넣을 array를 만들고
	TArray<TEnumAsByte<EObjectTypeQuery> > TargetObjects;
	//pawn만 검출되도록 한다(collision기준으로 pawn은 3번째 위치해있음)
	TargetObjects.Add(EObjectTypeQuery::ObjectTypeQuery3);
	TArray<AActor*> ActorToIgnore;
	TArray<AActor*> OutActors;

	//소리 감지 범위 생성
	bool bIsSuccessCapsuleOverlapActors =
		UKismetSystemLibrary::CapsuleOverlapActors(GetWorld(), GetActorLocation(),
			RadiusDetectSound, RadiusDetectSound, TargetObjects, nullptr, ActorToIgnore, OutActors);

	//생성이 되었으면
	if (bIsSuccessCapsuleOverlapActors)
	{
		//검출된 액터들리스트에서
		for (auto Object : OutActors)
		{
			//메인 캐릭터를 찾는다.
			APawn* ControlledCharacter = Cast<APawn>(Object);//메인 캐릭터를 검출할 변수
			//없으면 다음 액터로 넘어가고
			if (ControlledCharacter == nullptr)
			{
				continue;
			}
			//있으면
			else
			{
				//현재 플레이어가 조종하고 있는 pawn이 메인 캐릭터인지 확인하고
				if (GetWorld()->GetFirstPlayerController()->GetPawn() == ControlledCharacter)
				{
					//감지 범위에 들어왔다고 체크
					bIsInSoundCircle = true;
				}
			}
		}
	}
}

void AHumanEnemy::SetView()
{
	//기본은 false
	bIsInView = false;

	FVector CurPlayerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector MyPos = GetActorLocation();

	CurPlayerPos.Z = 0;
	MyPos.Z = 0;

	int Distance = (CurPlayerPos - MyPos).Size();
	if (Distance <= RadiusView)
	{
		//각도 계산
		FVector A = (CurPlayerPos - MyPos).GetSafeNormal();
		A.Z = 0;
		FVector B = GetActorForwardVector();
		B.Z = 0;
		float DotA = FVector::DotProduct(A, B);
		float Angle = FMath::Acos(DotA) * 180/PI;
		if (Angle <= AngleView)
		{
			//무언가에 가려져 있으면 검출이 안되도록 하는 LineTrace 설계
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
					//Hit 된 Actor가 플레이어라면
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
	//캡슐로 검출할 오브젝트를 넣을 array를 만들고
	TArray<TEnumAsByte<EObjectTypeQuery> > TargetObjects;
	//pawn만 검출되도록 한다(collision기준으로 pawn은 3번째 위치해있음)
	TargetObjects.Add(EObjectTypeQuery::ObjectTypeQuery3);
	TArray<AActor*> ActorToIgnore;
	TArray<AActor*> OutActors;

	//소리 감지 범위 생성
	bool bIsSuccessCapsuleOverlapActors =
		UKismetSystemLibrary::CapsuleOverlapActors(GetWorld(), GetActorLocation(),
			RadiusAttack, RadiusAttack, TargetObjects, nullptr, ActorToIgnore, OutActors);

	//생성이 되었으면
	if (bIsSuccessCapsuleOverlapActors)
	{
		//검출된 액터들리스트에서
		for (auto Object : OutActors)
		{
			//메인 캐릭터를 찾는다.
			APawn* ControlledCharacter = Cast<APawn>(Object);//메인 캐릭터를 검출할 변수
			//없으면 다음 액터로 넘어가고
			if (ControlledCharacter == nullptr)
			{
				continue;
			}
			//있으면
			else
			{
				//현재 플레이어가 조종하고 있는 pawn이 메인 캐릭터인지 확인하고
				if (GetWorld()->GetFirstPlayerController()->GetPawn() == ControlledCharacter)
				{
					//공격 범위에 들어왔다고 체크
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
	//플레이어 위치 설정
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
	//어그로 수치가 다른 상태로 진입할 수 있는 수치가 아니라면
	if (Aggression < 10)
	{
		//Watch를 유지
		return EEnemyState::Watch;
	}
	/*
	else if ((Aggression >= 10 && Aggression < 40) && bIsInView == false)
	{
		return EEnemyState::Search;
	}
	*/
	//Search
	//어그로 수치가 10이상 40미만일때(상승), 현재 상태가 경계라면
	else if ((Aggression >= 10 && Aggression < 40) && CurState == EEnemyState::Watch)
	{
		//시야에 플레이어가 없다면
		if (bIsInView == false)
		{
			//Search로 진입(Watch -> Search)
			return EEnemyState::Search;
		}
	}
	//어그로 수치가 10이상 40미만일때(하락), 현재 상태가 추적이라면
	else if ((Aggression >= 10 && Aggression < 40) && CurState == EEnemyState::Track)
	{
		//시야에 플레이어가 없다면
		if (bIsInView == false)
		{
			//Search로 진입(Track -> Search)
			return EEnemyState::Search;
		}
	}

	//Track
	//어그로 수치가 40이상 50미만일때(상승), 현재 상태가 수색이라면
	else if ((Aggression >= 40 && Aggression <= 50) && CurState == EEnemyState::Search)
	{
		//추적상태로 진입
		return EEnemyState::Track;
	}
	//현재 상태가 수색상태이고 시야에 플레이어가 있다면
	else if (CurState == EEnemyState::Search && bIsInView == true)
	{
		Aggression = 40;
		//추적상태로 진입
		return EEnemyState::Track;
	}
	//경계상태에서 어그로가 10 이상이고 시야에 플레이어가 있다면
	else if (Aggression < 10 && CurState == EEnemyState::Watch)
	{
		if (bIsInView == true)
		{
			Aggression = 40;
			return EEnemyState::Track;
		}
	}
	//피격시 추적으로 진입하는 코드 짜기

	
	//Attack
	//모든 상태에서 공격가능범위 안에 플레이어가 있다면 진입
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
	//현재 상태와 NewState가 다르다면(같다면 상태 바꿀필요 없음)
	if (CurState != NewState)
	{
		//현재 상태를 떠나고
		LeaveState(CurState);
		//NewState 진입
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
	//LeaveCurState가 경계면
	if (LeaveCurState == EEnemyState::Watch)
	{
		//그에 맞는 행동
	}

	//LeaveCurState가 수색이면
	if (LeaveCurState == EEnemyState::Search)
	{
		//그에 맞는 행동
	}

	//LeaveCurState가 추적이면
	if (LeaveCurState == EEnemyState::Track)
	{
		//그에 맞는 행동
	}

	//LeaveCurState가 공격이면
	if (LeaveCurState == EEnemyState::Attack)
	{
		//그에 맞는 행동
	}
}

void AHumanEnemy::EnterNewState(EEnemyState EnterNewState)
{
	//EnterNewState가 경계면
	if (EnterNewState == EEnemyState::Watch)
	{
		//그에 맞는 행동(함수로 만들자!)
		EnterWatch();
	}

	//EnterNewState가 수색이면
	if (EnterNewState == EEnemyState::Search)
	{
		//그에 맞는 행동(함수로 만들자!)
		EnterSearch();
	}

	//EnterNewState가 추적이면
	if (EnterNewState == EEnemyState::Track)
	{
		//그에 맞는 행동(함수로 만들자!)
		EnterTrack();
	}

	//EnterNewState가 공격이면
	if (EnterNewState == EEnemyState::Attack)
	{
		//그에 맞는 행동(함수로 만들자!)
		EnterAttack();
	}
}

//상태진입시 행동
void AHumanEnemy::EnterWatch()
{
	if (EnemySpeed.Contains(CurState))
	{
		Speed = EnemySpeed[CurState];
	}
	//아직 딱히 없음
}

//더 가까운 지점 확인하기
void AHumanEnemy::EnterSearch()
{
	bIsSearchX = false;
	bIsVisitX = false;
	bIsVisitY = false;
	WaitTime = 0.0f;
	SearchCount = 0;

	Search_PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	//Y가 더 가까우면
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
	//진입할때 어그로 수치를 40으로 지정
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

//상태별 행동
void AHumanEnemy::DoWatch()
{
	//행동
	//HUD변화

	//어그로 하락
	//어그로가 0이 아니면
	if (Aggression != 0)
	{
		//어그로 감소
		AggressionDown();
	}
}
void AHumanEnemy::DoSearch()
{
	//UE_LOG(LogTemp, Log, TEXT("DoSearch"));
	//X가 더 가까워서 X부터 방문
	if (bIsSearchX == true)
	{
		if (SearchCount == 10)
		{
			AAIController* AIController = Cast<AAIController>(GetController());
			if (AIController)
			{
				//최초스폰지점으로 이동
				Cast<AAIController>(GetController())->MoveToLocation(InitLocation);
			}

			//도착하면 어그로 새로 지정
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
			//X로 이동
			//시선 방향은 쫓아가는 방향
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
					//X로 이동
					Cast<AAIController>(GetController())->MoveToLocation(SearchPointX->GetComponentLocation());
				}
			}
		}
		else
		{
			//Y로 이동
			//시선 방향은 쫓아가는 방향
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
					//Y로 이동
					Cast<AAIController>(GetController())->MoveToLocation(SearchPointY->GetComponentLocation());
				}
			}

		}
	}
	//Y가 더 가까워서 Y부터 방문
	else
	{
		if (SearchCount == 10)
		{
			AAIController* AIController = Cast<AAIController>(GetController());
			if (AIController)
			{
				//최초스폰지점으로 이동
				Cast<AAIController>(GetController())->MoveToLocation(InitLocation);
			}

			//도착하면 어그로 새로 지정
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
			//Y로 이동
			//시선 방향은 쫓아가는 방향
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
					//Y로 이동
					Cast<AAIController>(GetController())->MoveToLocation(SearchPointY->GetComponentLocation());
				}
			}
		}
		else
		{
			//X로 이동
			//시선 방향은 쫓아가는 방향
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
					//X로 이동
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

	//시선방향은 플레이어위치
	//시선 방향은 목적지
	GetCharacterMovement()->bOrientRotationToMovement = false;

	FRotator EnemyRotator = GetActorRotation();
	FRotator ChangeRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerLocation);
	SetActorRotation(FRotator(EnemyRotator.Pitch, ChangeRotator.Yaw, EnemyRotator.Roll));

	if (AIController)
	{
		//플레이어 쫓아감
		Cast<AAIController>(GetController())->MoveToLocation(PlayerLocation);
	}

	//HUD변화

	//플레이어가 시야각에 없다면
	if (bIsInView == false)
	{
		//초당 10씩 어그로 감소
		AggressionDown();
	}
}

void AHumanEnemy::DoAttack()
{
	//시선은 플레이어에게 고정
	//최대3번 공격 가능
	
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
		//죽었을때 처리
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
	//초당 5증가
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