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
	//���������� ��׷ΰ� �߻��� ������ ���� ��׷ΰ� �߻��� ���� ��ġ�� �־��ֱ�
	//LastAggroLocation = AggroLocation;
	//���� ��׷� ��ġ�� ���� ��׷� ��ġ + �������Ѿ� �� ��׷� ��ġ
	Aggression = Aggression + IncreaseAggro;
	//���� ��׷� ��ġ�� �ִ��̶� ���ų� ũ�ٸ�
	if (Aggression >= MaxAggro)
	{
		//���� ��׷� ��ġ�� �ִ����� ����
		Aggression = MaxAggro;
	}
}

//���� ���¿� ���� ��׷� ���� ��ġ��ŭ ���� ��׷θ� ���ҽ�Ų��.
//���� ������ ȣ���ϴ� Ŭ�������� �˻��ϵ���
void AEnemy::AggressionDown()
{
	//���� ���¿� ���� ��׷ΰ��� ��ġ�� ������ ���� �ʾҴٸ� 0����(���� ����)
	float DownAggro = 0;
	//���� ���¿� ���� ��׷� ���� ��ġ�� ������ �־����� Ȯ���ϰ�,(���� ����)
	if (EnemyDownAggro.Contains(CurState))
	{
		//�ִٸ� ���� ���¿� ���� ��׷� ���� ��ġ��ŭ
		DownAggro = EnemyDownAggro[CurState];
	}
	//����
	Aggression  = Aggression - UGameplayStatics::GetWorldDeltaSeconds(GetWorld())*DownAggro;
	//��׷ΰ� 0���Ϸ� �������� �ʵ��� ����
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