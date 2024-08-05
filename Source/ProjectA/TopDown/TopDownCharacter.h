#pragma once

#include "CoreMinimal.h"
#include "Character/C_Common.h"
#include "TopDownCharacter.generated.h"

UCLASS()
class PROJECTA_API ATopDownCharacter : public AC_Common
{
	GENERATED_BODY()

public:
	ATopDownCharacter(const class FObjectInitializer& ObjectInitializer);
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parts, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* RWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TScriptInterface<class II_PlayerAnim> AnimInterface;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parts, meta = (AllowPrivateAccess = "true"))
	class UInputConfig* InputConfig;
protected:
	virtual void BeginPlay() override;
	void Move(const struct FInputActionValue& ActionValue);
	void EndMove();
	void Jump();
	void Attack();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	virtual void Tick(float DeltaTime) override;	

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return SpringArm; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
