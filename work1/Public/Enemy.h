// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class USceneCaptureComponent2D;

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Watch,//경계
	Search,//수색
	Track,//추적
	Attack,//공격
	BeAttacked,//피격
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
	//AggressionUp에 사용되는 변수들
	//Enemy의 현재 어그로수치를 저장
	UPROPERTY(VisibleAnywhere, Category = "BaseProperty")
	float Aggression;
	//어그로가 발생한 마지막 위치를 저장
	FVector LastAggroLocation;
	//어그로의 최대 수치를 저장
	float MaxAggro = 50;

	//적 사망여부저장
	bool Killed;
	
	//현재 적 상태
	UPROPERTY(VisibleAnywhere, Category = "BaseProperty")
	EEnemyState CurState;

	//상태에 따른 어그로 감소 수치를 정함
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TMap<EEnemyState, float> EnemyDownAggro;

	//상태에 따른 이동속도
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TMap<EEnemyState, float> EnemySpeed;

	//수색위치
	UPROPERTY(VisibleAnywhere, Category = "SearchPoiont")
	USceneComponent* SearchPointX;
	UPROPERTY(VisibleAnywhere, Category = "SearchPoiont")
	USceneComponent* SearchPointY;
	//최초스폰지점
	FVector InitLocation;


public:
	UFUNCTION(BlueprintCallable, Category = "HideThings")
	virtual void HideThings(USceneCaptureComponent2D* SceneComponent) {}

public:
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	float MyViewAggro;//시야에 들어왔을때 증가시키는 어그로 수치, 비긴플레이에서 초기화해주기!

	//어그로 수치를 증가시키는 함수
	UFUNCTION(BlueprintCallable, Category = "Aggression")
	void AggressionUp(float IncreaseAggro);

	//어그로 수치를 감소시키는 함수
	UFUNCTION(BlueprintCallable, Category = "Aggression")
	void AggressionDown();

	//상태 제어에 관한 함수들
	virtual void ChangeState() {}
	virtual EEnemyState CheckConditionState() { return CurState; }
	virtual void SetState(EEnemyState NewState) {}
	virtual void LeaveState(EEnemyState LeaveCurState) {}
	virtual void EnterNewState(EEnemyState EnterNewState) {}

	UFUNCTION(BlueprintCallable, Category = "GetterState")
	EEnemyState GetCurState() { return CurState; }

	//특수 상태(피격, 사망)
	virtual void OnDamage(float Damage) {}
	virtual void OnDeath();

public:
	//소리 감지 범위 만들기
	virtual void SetDetectingSound() {}
	//시야 만들기
	virtual void SetView() {}
	//공격 범위 만들기
	virtual void SetAttackCircle() {}
	//플레이어가 적의 소리 감지 범위 안에 들어왔는지 알려주는 변수를 리턴
	//UFUNCTION(BlueprintCallable, Category = "GetterEnemy")
	virtual bool IsInSoundCircle() { return true; }
	//UFUNCTION(BlueprintCallable, Category = "GetterEnemy")
	virtual bool IsInView() { return true; }

	//현재 어그로 반환
	UFUNCTION(BlueprintCallable, Category = "GetAggression")
	int GetAggression();

public:
	//공격 애니메이션 재생 시작
	UFUNCTION(BlueprintCallable, Category = "EnemyAnimFunc")
	virtual void OnStartAttack() {}
	//공격 애니메이션 재생 끝
	UFUNCTION(BlueprintCallable, Category = "EnemyAnimFunc")
	virtual void OnEndAttack() {}
	//피격 애니메이션 재생 끝
	UFUNCTION(BlueprintCallable, Category = "EnemyAnimFunc")
	virtual void OnEndAttacked() {}

public:
	UFUNCTION(BlueprintCallable, Category = "KillEnemy")
	virtual void StealthKill() {}

protected:
	//플레이어가 적의 소리 감지 범위 안에 들어왔는지 알려주는 변수
	bool bIsInSoundCircle;
	//플레이어가 적의 시야 범위 안에 들어왔는지 알려주는 변수
	bool bIsInView;
	//플레이어가 적의 공격 범위 안에 들어왔는지 알려주는 변수
	bool bIsInAttack;

	//피격상태를 재생해야 함
	bool PlayBeAttackedAnim;

public:
	UFUNCTION(BlueprintCallable, Category = "KillEnemy")
	bool GetKilled();
};
