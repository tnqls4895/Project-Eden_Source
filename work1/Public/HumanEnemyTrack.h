// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HumanEnemy.h"
#include "HumanEnemyTrack.generated.h"

/**
 * 
 */
UCLASS()
class WORK1_API AHumanEnemyTrack : public AHumanEnemy
{
	GENERATED_BODY()

	
public:
	virtual EEnemyState CheckConditionState() override;

	virtual void DoTrack() override;
};
