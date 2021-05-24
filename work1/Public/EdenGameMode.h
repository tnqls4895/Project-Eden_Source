// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EdenGameMode.generated.h"

class AHumanEnemy;
class AEnemy;

UCLASS()
class WORK1_API AEdenGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	
	void AddEnemy(AEnemy* NewEnemy);
	void RemoveEnemy(AEnemy* OldEnemy);

	UFUNCTION(BlueprintCallable, Category = "EdenGameMode")
	void AggroUp(float Aggro);
	UFUNCTION(BlueprintCallable, Category = "EdenGameMode")
	TArray<AEnemy*>& GetEnemyList();

private:
	TArray<AEnemy*> EnemyArray;
};
