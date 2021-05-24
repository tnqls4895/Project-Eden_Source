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

	//�������������Ʈ��
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* PipeBoxComponent;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* PipeMeshComponent;

	//�⺻ ������(�ϴ� �ٰŸ��ΰ����� �������� �ۼ�.���� �����Ǹ� ����)->������ �Ǵ� ����÷��̿��� �ʱ�ȭ �������
	UPROPERTY(VisibleAnywhere, Category = "BaseProperty")
	float HP = 50;//ü��
	UPROPERTY(BlueprintReadOnly)
	float MaxHP = 50;//�ִ�ü��
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	float Speed = 0.f;//�̵��ӵ�
	FString Weapon = "Pipe";//��������
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	float WeaponDamage = 7.5;//�������� ������
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int AngleView = 70;//�þ߰�
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int RadiusView = 1000;//�þ߹�����
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int AngleTurnView = 25;//ȸ���� �� �ִ� �þ� ����
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int RadiusDetectSound = 500;//�Ҹ� ���� ���� ������
	UPROPERTY(EditAnywhere, Category = "BaseProperty")
	int RadiusAttack = 100;//���� ���� ������
	UPROPERTY(BlueprintReadOnly)
	bool bCanStealthKill = false;

	UPROPERTY(EditAnywhere, Category = "SearchEnemy")
	bool OnlySearch = false;


	FVector Search_PlayerLocation;

	//X�� �湮�ؾ� �ϴ°�?true�� X�� �湮�ϱ�
	bool bIsSearchX;
	//X�� �湮�ߴ°�?
	bool bIsVisitX;
	//Y�� �湮�ߴ°�?
	bool bIsVisitY;


	//�Ҹ� ���� ���� �����
	virtual void SetDetectingSound() override;
	//�þ� �����
	virtual void SetView() override;
	//���� ���� �����
	virtual void SetAttackCircle() override;
	//�÷��̾ ���� �Ҹ� ���� ���� �ȿ� ���Դ��� �˷��ִ� ������ ����
	UFUNCTION(BlueprintCallable, Category = "GetterEnemy")
	virtual bool IsInSoundCircle() override;
	UFUNCTION(BlueprintCallable, Category = "GetterEnemy")
	virtual bool IsInView() override;

	//�þ߾ȿ� ������ ��׷� �ø��� �Լ�
	void ViewAggressionUp();

	//�÷��̾� ��ġ�� �����ϴ� �Լ�(���� �ܰ迡�� ���)
	//UFUNCTION(BlueprintCallable, Category = "GetterHumanEnemy")
	//FVector GetLocationAPlayer();
	//�÷��̾� ��ġ�� ����
	//UFUNCTION(BlueprintCallable, Category = "SetterHumanEnemy")
	//void SetLocationAPlayer();

	//���� ��� ���� �Լ���
	virtual void ChangeState() override;
	virtual EEnemyState CheckConditionState() override;
	virtual void SetState(EEnemyState NewState) override;
	virtual void LeaveState(EEnemyState LeaveCurState) override;
	virtual void EnterNewState(EEnemyState EnterNewState) override;

	//���� ����
	void EnterWatch();
	void EnterSearch();
	void EnterTrack();
	void EnterAttack();
	void EnterBeAttacked();

	//���º� �ൿ ȣ��
	void DoState();

	//���º� �ൿ
	void DoWatch();
	void DoSearch();
	virtual void DoTrack();
	void DoAttack();

	//Ư������
	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual void OnDamage(float Damage) override;

	//��������
	virtual void StealthKill() override;

	//�ִϸ��̼ǰ���
	virtual void OnStartAttack() override;
	virtual void OnEndAttack() override;
	virtual void OnEndAttacked() override;


	//HP����
	float GetHP();

	virtual void OnDeath() override;

	//hide
	virtual void HideThings(USceneCaptureComponent2D* SceneComponent) override;

	//����������
	UFUNCTION(BlueprintCallable, Category = "HumenEnemyGetter")
	bool GetIsAttacking();

protected:

	//searchCount
	int SearchCount;
	//���� Ƚ��
	int AttackCount;

	//��� ����
	//bool IsDeath;
	//������ �� �ִ���(����Ÿ��)
	bool bCanAttack;
	//���� �ִ� ���������
	bool bIsPlayingAttackAnim;

	//�����Ҷ� ���
	float WaitTime;

};
