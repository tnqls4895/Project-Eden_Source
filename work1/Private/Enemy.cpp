// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "EdenGameMode.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Aggression = 0;
	CurState = EEnemyState::Watch;

	Killed = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	AEdenGameMode* EdenGameMode = Cast<AEdenGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (EdenGameMode != nullptr)
	{
		EdenGameMode->AddEnemy(this);
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

int AEnemy::GetAggression()
{
	return Aggression;
}

void AEnemy::AggressionUp(float IncreaseAggro)
{
	//마지막으로 어그로가 발생한 지점에 현재 어그로가 발생한 지점 위치를 넣어주기
	//LastAggroLocation = AggroLocation;
	//현재 어그로 수치는 현재 어그로 수치 + 증가시켜야 할 어그로 수치
	Aggression = Aggression + IncreaseAggro;
	//현재 어그로 수치가 최댓값이랑 같거나 크다면
	if (Aggression >= MaxAggro)
	{
		//현재 어그로 수치를 최댓값으로 갱신
		Aggression = MaxAggro;
	}
}

//현재 상태에 따른 어그로 감소 수치만큼 현재 어그로를 감소시킨다.
//실행 조건은 호출하는 클래스에서 검사하도록
void AEnemy::AggressionDown()
{
	//현재 상태에 따른 어그로감소 수치를 지정해 주지 않았다면 0감소(에러 방지)
	float DownAggro = 0;
	//현재 상태에 따른 어그로 감소 수치를 지정해 주었는지 확인하고,(에러 방지)
	if (EnemyDownAggro.Contains(CurState))
	{
		//있다면 현재 상태에 따른 어그로 감소 수치만큼
		DownAggro = EnemyDownAggro[CurState];
	}
	//감소
	Aggression  = Aggression - UGameplayStatics::GetWorldDeltaSeconds(GetWorld())*DownAggro;
	//어그로가 0이하로 내려가지 않도록 설정
	if (Aggression < 0)
	{
		Aggression = 0;
	}
}

void AEnemy::OnDeath()
{
	AEdenGameMode* EdenGameMode = Cast<AEdenGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (EdenGameMode != nullptr)
	{
		EdenGameMode->RemoveEnemy(this);
	}
}

bool AEnemy::GetKilled()
{
	return Killed;
}