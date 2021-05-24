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

	return EEnemyState::Track;
}

void AHumanEnemyTrack::DoTrack()
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
	/*
	if (bIsInView == false)
	{
		//�ʴ� 10�� ��׷� ����
		AggressionDown();
	}
	*/
}