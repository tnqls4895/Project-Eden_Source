// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanEnemyTrack.h"
#include "Kismet/KismetMathLibrary.h"
#include "EdenGameMode.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

EEnemyState AHumanEnemyTrack::CheckConditionState()
{
	if (PlayBeAttackedAnim)
	{
		if (CurState == EEnemyState::Watch || CurState == EEnemyState::Search)
		{
			Aggression = 40;
			return EEnemyState::Track;
		}
		//return EEnemyState::BeAttacked;
	}

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

	return EEnemyState::Track;
}

void AHumanEnemyTrack::DoTrack()
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
	/*
	if (bIsInView == false)
	{
		//초당 10씩 어그로 감소
		AggressionDown();
	}
	*/
}