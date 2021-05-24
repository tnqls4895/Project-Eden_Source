// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class USceneCaptureComponent2D;

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Watch,//���
	Search,//����
	Track,//����
	Attack,//����
	BeAttacked,//�ǰ�
};

UCLASS()
class WORK1_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	//AggressionUp�� ���Ǵ� ������
	//Enemy�� ���� ��׷μ�ġ�� ����
	UPROPERTY(VisibleAnywhere, Category = "BaseProperty")
	float Aggression;
	//��׷ΰ� �߻��� ������ ��ġ�� ����
	FVector LastAggroLocation;
	//��׷��� �ִ� ��ġ�� ����
	float MaxAggro = 50;

	//�� �����������
	bool Killed;
	
	//���� �� ����
	UPROPERTY(VisibleAnywhere, Category = "BaseProperty")
	EEnemyState CurState;

	//���¿� ���� ��׷� ���� ��ġ�� ����
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TMap<EEnemyState, float> EnemyDownAggro;

	//���¿� ���� �̵��ӵ�
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TMap<EEnemyState, float> EnemySpeed;

	//������ġ
	UPROPERTY(VisibleAnywhere, Category = "SearchPoiont")
	USceneComponent* SearchPointX;
	UPROPERTY(VisibleAnywhere, Category = "SearchPoiont")
	USceneComponent* SearchPointY;
	//���ʽ�������
	FVector InitLocation;


public:
	UFUNCTION(BlueprintCallable, Category = "HideThings")
	virtual void HideThings(USceneCaptureComponent2D* SceneComponent) {}

public:
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	float MyViewAggro;//�þ߿� �������� ������Ű�� ��׷� ��ġ, ����÷��̿��� �ʱ�ȭ���ֱ�!

	//��׷� ��ġ�� ������Ű�� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Aggression")
	void AggressionUp(float IncreaseAggro);

	//��׷� ��ġ�� ���ҽ�Ű�� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Aggression")
	void AggressionDown();

	//���� ��� ���� �Լ���
	virtual void ChangeState() {}
	virtual EEnemyState CheckConditionState() { return CurState; }
	virtual void SetState(EEnemyState NewState) {}
	virtual void LeaveState(EEnemyState LeaveCurState) {}
	virtual void EnterNewState(EEnemyState EnterNewState) {}

	UFUNCTION(BlueprintCallable, Category = "GetterState")
	EEnemyState GetCurState() { return CurState; }

	//Ư�� ����(�ǰ�, ���)
	virtual void OnDamage(float Damage) {}
	virtual void OnDeath();

public:
	//�Ҹ� ���� ���� �����
	virtual void SetDetectingSound() {}
	//�þ� �����
	virtual void SetView() {}
	//���� ���� �����
	virtual void SetAttackCircle() {}
	//�÷��̾ ���� �Ҹ� ���� ���� �ȿ� ���Դ��� �˷��ִ� ������ ����
	//UFUNCTION(BlueprintCallable, Category = "GetterEnemy")
	virtual bool IsInSoundCircle() { return true; }
	//UFUNCTION(BlueprintCallable, Category = "GetterEnemy")
	virtual bool IsInView() { return true; }

	//���� ��׷� ��ȯ
	UFUNCTION(BlueprintCallable, Category = "GetAggression")
	int GetAggression();

public:
	//���� �ִϸ��̼� ��� ����
	UFUNCTION(BlueprintCallable, Category = "EnemyAnimFunc")
	virtual void OnStartAttack() {}
	//���� �ִϸ��̼� ��� ��
	UFUNCTION(BlueprintCallable, Category = "EnemyAnimFunc")
	virtual void OnEndAttack() {}
	//�ǰ� �ִϸ��̼� ��� ��
	UFUNCTION(BlueprintCallable, Category = "EnemyAnimFunc")
	virtual void OnEndAttacked() {}

public:
	UFUNCTION(BlueprintCallable, Category = "KillEnemy")
	virtual void StealthKill() {}

protected:
	//�÷��̾ ���� �Ҹ� ���� ���� �ȿ� ���Դ��� �˷��ִ� ����
	bool bIsInSoundCircle;
	//�÷��̾ ���� �þ� ���� �ȿ� ���Դ��� �˷��ִ� ����
	bool bIsInView;
	//�÷��̾ ���� ���� ���� �ȿ� ���Դ��� �˷��ִ� ����
	bool bIsInAttack;

	//�ǰݻ��¸� ����ؾ� ��
	bool PlayBeAttackedAnim;

public:
	UFUNCTION(BlueprintCallable, Category = "KillEnemy")
	bool GetKilled();
};
