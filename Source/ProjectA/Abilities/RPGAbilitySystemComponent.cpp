// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGAbilitySystemComponent.h"
#include "RPGGameplayAbility.h"

void URPGAbilitySystemComponent::ReceiveDamage(URPGAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}

void URPGAbilitySystemComponent::BindAbilityActivationToInputComponent(UInputComponent* InputComponent, FGameplayAbilityInputBinds BindInfo)
{
    Super::BindAbilityActivationToInputComponent(InputComponent, BindInfo);

	// �������
	/*
    if (bAllowAllInput)
    {
        // ��� �Է��� ����ϵ��� ���ε��մϴ�.
        InputComponent->BindAction(FName("IA_Confirm"), EInputEvent::IE_Pressed, this, &URPGAbilitySystemComponent::AbilityLocalInputPressed,0);
        BindInfo.BindAction("IA_Confirm", EInputEvent::IE_Released);
        BindInfo.BindAction("IA_Cancel", EInputEvent::IE_Pressed);
        BindInfo.BindAction("IA_Cancel", EInputEvent::IE_Released);
        // �ʿ��� ��� �Է¿� ���� �߰� ���ε��մϴ�.
        BindInfo.BindAction("IA_Jump", EInputEvent::IE_Pressed);
        BindInfo.BindAction("IA_Attack", EInputEvent::IE_Pressed);
        BindInfo.BindAction("IA_SkillQ", EInputEvent::IE_Pressed);
        BindInfo.BindAction("IA_SkillE", EInputEvent::IE_Pressed);
        BindInfo.BindAction("IA_SkillR", EInputEvent::IE_Pressed);
        BindInfo.BindAction("IA_Skill1", EInputEvent::IE_Pressed);
        BindInfo.BindAction("IA_Skill2", EInputEvent::IE_Pressed);
        BindInfo.BindAction("IA_Skill3", EInputEvent::IE_Pressed);
        BindInfo.BindAction("IA_Skill4", EInputEvent::IE_Pressed);
        BindInfo.BindAction("IA_SkillMUp", EInputEvent::IE_Pressed);
        BindInfo.BindAction("IA_SkillMDown", EInputEvent::IE_Pressed);
        BindInfo.BindAction("IA_LiquidZ", EInputEvent::IE_Pressed);
        BindInfo.BindAction("IA_LiquidX", EInputEvent::IE_Pressed);
        BindInfo.BindAction("IA_LiquidC", EInputEvent::IE_Pressed);
        BindInfo.BindAction("IA_LiquidV", EInputEvent::IE_Pressed);
        BindInfo.BindAction("IA_Move", EInputEvent::IE_Axis);
        BindInfo.BindAction("IA_Look", EInputEvent::IE_Axis);
    }*/
}

void URPGAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
				{
					/*InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
					InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);*/
					TryActivateAbilitiesByTag(FGameplayTagContainer(InputTag));
				}
			}
			
		}
	}
}

void URPGAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				/*InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);*/
				AbilitySpecInputReleased(AbilitySpec);
			}
		}
	}
}

//����� ����Ϸ��� �Ҽ������� �Ⱦ�����
void URPGAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
    Super::AbilitySpecInputPressed(Spec);

    //UGameplayAbility::bReplicateInputDirectly�� �������� �ʽ��ϴ�.
    //��� WaitInputPress ability task �� �۵��ϵ��� ��� ������ �̺�Ʈ�� ����մϴ�.
    //if (Spec.IsActive())
    //{
    //    //Invoke the InputPressed event. This is not replicated here. If Someone is listening, they may replicate the InputPressed event to the server.
    //    InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
    //}	
}

//����� ����Ϸ��� �Ҽ������� �Ⱦ�����
void URPGAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
    Super::AbilitySpecInputReleased(Spec);

    //UGameplayAbility::bReplicateInputDirectly�� �������� �ʽ��ϴ�.
    //��� WaitInputRelease ability task �� �۵��ϵ��� ��� ������ �̺�Ʈ�� ����մϴ�.
    //if (Spec.IsActive())
    //{
    //    //Invoke the InputReleased event. This is not replicated here. If Someone is listening, they may replicate the InputReleased event to the server.
    //    InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
    //}
	InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
}

void URPGAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	/*if (HasMatchingGameplayTag(TAG_Gameplay_AbilityInputBlocked))
	{
		ClearAbilityInput();
		return;
	}*/

	/*static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();*/

	/*&TODO : See if we can use FScopedServerAbilityRPCBatcher ScopedRPCBatcher in some of these loops
	Process all abilities that acitvate when the input is held.*/

	//for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	//{
	//	if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
	//	{
	//		if (AbilitySpec->Ability && !AbilitySpec->IsActive())
	//		{
	//			const URPGGameplayAbility* AbilityCDO = CastChecked<URPGGameplayAbility>(AbilitySpec->Ability);

	//			if (AbilityCDO->GetActivationPolicy() == ERPGAbilityActivationPolicy::WhileInputActive)
	//			{
	//				AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
	//			}
	//		}
	//	}
	//}
	////Process all abilities that had their input pressed this frame.
	//for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	//{
	//	if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
	//	{
	//		if (AbilitySpec->Ability)
	//		{
	//			AbilitySpec->InputPressed = true;

	//			if (AbilitySpec->IsActive())
	//				AbilitySpecInputPressed(*AbilitySpec);
	//			else
	//			{
	//				const URPGGameplayAbility* AbilityCDO = CastChecked<URPGGameplayAbility>(AbilitySpec->Ability);

	//				if (AbilityCDO->GetActivationPolicy() == ERPGAbilityActivationPolicy::OnOnputTriggered)
	//					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
	//			}
	//		}
	//	}
	//}

	////Try to activate all the abilities that are form presses and holds.
	////We do it all at once so that held inputs don't activate the ability 
	////and then also send a input event to the ability because of the press.
	//for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	//{
	//	TryActivateAbility(AbilitySpecHandle);
	//}

	////Process all abilities that had their input released this frame.
	//for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	//{
	//	if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
	//	{
	//		if (AbilitySpec->Ability)
	//		{
	//			AbilitySpec->InputPressed = false;

	//			if (AbilitySpec->IsActive())
	//			{
	//				//Ability is active so pass along the input event.
	//				UUtils::PrintScreen(GetWorld(), FString::Print(TEXT("AbilitySpecInputReleased")));
	//				AbilitySpecInputReleased(*AbilitySpec);
	//				//CancleAbilityHandle(SpecHandle);
	//			}
	//		}
	//	}
	//}
	//// Clear the cached ability handles.
	//InputPressedSpecHandles.Reset();
	//InputReleasedSpecHandles.Reset();
}
