#include "EdenGameMode.h"
#include "Enemy.h"

void AEdenGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AEdenGameMode::AddEnemy(AEnemy* NewEnemy)
{
	EnemyArray.Add(NewEnemy);
}

void AEdenGameMode::RemoveEnemy(AEnemy* OldEnemy)
{
	EnemyArray.Remove(OldEnemy);
}

void AEdenGameMode::AggroUp(float Aggro)
{
	for (AEnemy* enemy : EnemyArray)
	{
		if (enemy->IsInSoundCircle() == true && enemy->IsInView() == false)
		{
			enemy->AggressionUp(Aggro);
		}
	}
}

TArray<AEnemy*>& AEdenGameMode::GetEnemyList()
{
	return EnemyArray;
}