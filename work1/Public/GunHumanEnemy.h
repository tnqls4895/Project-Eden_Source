// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "GunHumanEnemy.generated.h"

/**
 * 
 */
UCLASS()
class WORK1_API AGunHumanEnemy : public AEnemy
{
	GENERATED_BODY()
	

public:

	//기본 변수들(일단 근거리인간형적 기준으로 작성.추후 문제되면 수정)->생성자 또는 비긴플레이에서 초기화 꼭해줘라
	UPROPERTY(VisibleAnywhere, Category = "BaseProperty")
	float HP = 40.0f;//체력
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	float Speed = 5.0f;//이동속도
	FString Weapon = "Gun";//장착무기
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	float WeaponDamage = 10.0f;//장착무기 데미지
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int AngleView = 70;//시야각
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int RadiusView = 1000;//시야반지름
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int AngleTurnView = 25;//회전할 수 있는 시야 각도
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int RadiusDetectSound = 500;//소리 감지 범위 반지름
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int RadiusAttack = 1000;//공격 범위 반지름
	
	
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int Magazine = 3;//보유탄창
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int Bullet = 10;//보유총알

};
