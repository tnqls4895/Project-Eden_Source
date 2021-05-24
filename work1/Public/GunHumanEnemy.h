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

	//�⺻ ������(�ϴ� �ٰŸ��ΰ����� �������� �ۼ�.���� �����Ǹ� ����)->������ �Ǵ� ����÷��̿��� �ʱ�ȭ �������
	UPROPERTY(VisibleAnywhere, Category = "BaseProperty")
	float HP = 40.0f;//ü��
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	float Speed = 5.0f;//�̵��ӵ�
	FString Weapon = "Gun";//��������
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	float WeaponDamage = 10.0f;//�������� ������
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int AngleView = 70;//�þ߰�
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int RadiusView = 1000;//�þ߹�����
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int AngleTurnView = 25;//ȸ���� �� �ִ� �þ� ����
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int RadiusDetectSound = 500;//�Ҹ� ���� ���� ������
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int RadiusAttack = 1000;//���� ���� ������
	
	
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int Magazine = 3;//����źâ
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int Bullet = 10;//�����Ѿ�

};
