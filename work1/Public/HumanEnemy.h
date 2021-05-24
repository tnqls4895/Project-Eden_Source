#pragma once


#include "CoreMinimal.h"
#include "Enemy.h"
#include "HumanEnemy.generated.h"


UCLASS()
class WORK1_API AHumanEnemy : public AEnemy
{
	GENERATED_BODY()

public:
	AHumanEnemy();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	//무기관련컴포넌트들
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* PipeBoxComponent;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* PipeMeshComponent;

	//기본 변수들(일단 근거리인간형적 기준으로 작성.추후 문제되면 수정)->생성자 또는 비긴플레이에서 초기화 꼭해줘라
	UPROPERTY(VisibleAnywhere, Category = "BaseProperty")
	float HP = 50;//체력
	UPROPERTY(BlueprintReadOnly)
	float MaxHP = 50;//최대체력
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	float Speed = 0.f;//이동속도
	FString Weapon = "Pipe";//장착무기
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	float WeaponDamage = 7.5;//장착무기 데미지
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int AngleView = 70;//시야각
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int RadiusView = 1000;//시야반지름
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int AngleTurnView = 25;//회전할 수 있는 시야 각도
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int RadiusDetectSound = 500;//소리 감지 범위 반지름
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int RadiusAttack = 100;//공격 범위 반지름
	UPROPERTY(BlueprintReadOnly)
	bool bCanStealthKill = false;

	UPROPERTY(EditAnywhere, Category = "SearchEnemy")
	bool OnlySearch = false;


	FVector Search_PlayerLocation;

	//X를 방문해야 하는가?true면 X를 방문하기
	bool bIsSearchX;
	//X를 방문했는가?
	bool bIsVisitX;
	//Y를 방문했는가?
	bool bIsVisitY;


	//소리 감지 범위 만들기
	virtual void SetDetectingSound() override;
	//시야 만들기
	virtual void SetView() override;
	//공격 범위 만들기
	virtual void SetAttackCircle() override;
	//플레이어가 적의 소리 감지 범위 안에 들어왔는지 알려주는 변수를 리턴
	UFUNCTION(BlueprintCallable, Category = "GetterEnemy")
	virtual bool IsInSoundCircle() override;
	UFUNCTION(BlueprintCallable, Category = "GetterEnemy")
	virtual bool IsInView() override;

	//시야안에 있으면 어그로 올리는 함수
	void ViewAggressionUp();

	//플레이어 위치를 리턴하는 함수(수색 단계에서 사용)
	//UFUNCTION(BlueprintCallable, Category = "GetterHumanEnemy")
	//FVector GetLocationAPlayer();
	//플레이어 위치를 저장
	//UFUNCTION(BlueprintCallable, Category = "SetterHumanEnemy")
	//void SetLocationAPlayer();

	//상태 제어에 관한 함수들
	virtual void ChangeState() override;
	virtual EEnemyState CheckConditionState() override;
	virtual void SetState(EEnemyState NewState) override;
	virtual void LeaveState(EEnemyState LeaveCurState) override;
	virtual void EnterNewState(EEnemyState EnterNewState) override;

	//상태 들어갈때
	void EnterWatch();
	void EnterSearch();
	void EnterTrack();
	void EnterAttack();
	void EnterBeAttacked();

	//상태별 행동 호출
	void DoState();

	//상태별 행동
	void DoWatch();
	void DoSearch();
	virtual void DoTrack();
	void DoAttack();

	//특수상태
	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual void OnDamage(float Damage) override;

	//배후제압
	virtual void StealthKill() override;

	//애니메이션관련
	virtual void OnStartAttack() override;
	virtual void OnEndAttack() override;
	virtual void OnEndAttacked() override;


	//HP리턴
	float GetHP();

	virtual void OnDeath() override;

	//hide
	virtual void HideThings(USceneCaptureComponent2D* SceneComponent) override;

	//공격중인지
	UFUNCTION(BlueprintCallable, Category = "HumenEnemyGetter")
	bool GetIsAttacking();

protected:

	//searchCount
	int SearchCount;
	//공격 횟수
	int AttackCount;

	//사망 여부
	//bool IsDeath;
	//공격할 수 있는지(딜쿨타임)
	bool bCanAttack;
	//공격 애님 재생중인지
	bool bIsPlayingAttackAnim;

	//수색할때 사용
	float WaitTime;

};
