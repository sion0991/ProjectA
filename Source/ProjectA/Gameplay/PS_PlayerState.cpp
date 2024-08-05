// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_PlayerState.h"
#include "Data/InventorySystemComponent.h"
#include "Abilities/RPGAbilitySystemComponent.h"
#include "Abilities/RPGAttributeSet.h"
#include "Data/ST_Item.h"
#include "UI/UW_MainHUD.h"
#include "PC_Player.h"
#include "Character/Player/C_Player.h"

APS_PlayerState::APS_PlayerState()
{
	InventorySystemComponent = CreateDefaultSubobject<UInventorySystemComponent>(TEXT("InventorySystemComponent"));
	InventorySystemComponent->SetIsReplicated(true);

	AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<URPGAttributeSet>(TEXT("AttributeSet"));

	//PlayerState�� NetUpdateFrequency�� ĳ���Ϳ� �����ϰ� ������
	//PlayerStates�� ��� �⺻���� ���� AbilitySystem���� �νĵǴ� ������ ������.
	//100�� ��� ���ӿ��� ���� �� �����Ƿ� ���ӿ����� ����
	NetUpdateFrequency = 100.0f;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

void APS_PlayerState::BeginPlay()
{
	HPChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHPAttribute()).AddUObject(this, &APS_PlayerState::HPChanged);
	MaxHPChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHPAttribute()).AddUObject(this, &APS_PlayerState::MaxHPChanged);
	HPRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHPRegenRateAttribute()).AddUObject(this, &APS_PlayerState::HPRegenRateChanged);
	MPChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMPAttribute()).AddUObject(this, &APS_PlayerState::MPChanged);
	MaxMPChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxMPAttribute()).AddUObject(this, &APS_PlayerState::MaxMPChanged);
	MPRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMPRegenRateAttribute()).AddUObject(this, &APS_PlayerState::MPRegenRateChanged);
	APChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetAPAttribute()).AddUObject(this, &APS_PlayerState::APChanged);
	MaxAPChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxAPAttribute()).AddUObject(this, &APS_PlayerState::MaxAPChanged);
	APRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetAPRegenRateAttribute()).AddUObject(this, &APS_PlayerState::APRegenRateChanged);
	XPChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetEXPAttribute()).AddUObject(this, &APS_PlayerState::EXPChanged);
	GoldChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetGoldAttribute()).AddUObject(this, &APS_PlayerState::GoldChanged);
	CharacterLevelChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetCharacterLevelAttribute()).AddUObject(this, &APS_PlayerState::CharacterLevelChanged);
	DamageChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetDamageAttribute()).AddUObject(this, &APS_PlayerState::DamageChanged);
	DeffenseChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetDeffenseAttribute()).AddUObject(this, &APS_PlayerState::DeffenseChanged);
	StrengthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStrengthAttribute()).AddUObject(this, &APS_PlayerState::StrengthChanaged);
	AgilityChangedDelegatedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetAgilityAttribute()).AddUObject(this, &APS_PlayerState::AgilityChanged);
	IntelligenceChangedDelegatedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetIntelligenceAttribute()).AddUObject(this, &APS_PlayerState::IntelligenceChanged);
	LuckChangedDelegatedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetLuckAttribute()).AddUObject(this, &APS_PlayerState::LuckChanged);

	// Tag change callbacks
	AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APS_PlayerState::StunTagChanged);
}

UInventorySystemComponent* APS_PlayerState::GetInventorySystemComponent() const
{
	return InventorySystemComponent;
}

FInventory* APS_PlayerState::GetInventory()
{
	return &Inventory;
}
UAbilitySystemComponent* APS_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

URPGAttributeSet* APS_PlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

void APS_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APS_PlayerState, Inventory);
}
bool APS_PlayerState::IsAlive() const
{
	return GetHP() > 0.0f;
}

void APS_PlayerState::ShowAbilityConfirmCancelText(bool ShowText)
{
	APC_Player* PC = Cast<APC_Player>(GetOwner());
	if (PC)
	{
		/*AHUD* HUD = PC->GetHUD();
		if (HUD)
			HUD->ShowAbilityConfirmCancelText(ShowText);*/
	}
}

float APS_PlayerState::GetHP() const
{
	return AttributeSet->GetHP();
}

float APS_PlayerState::GetMaxHP() const
{
	return AttributeSet->GetMaxHP();
}

float APS_PlayerState::GetHPRegenRate() const
{
	return AttributeSet->GetHPRegenRate();
}

float APS_PlayerState::GetMP() const
{
	return AttributeSet->GetMP();
}

float APS_PlayerState::GetMaxMP() const
{
	return AttributeSet->GetMaxMP();
}

float APS_PlayerState::GetMPRegenRate() const
{
	return AttributeSet->GetMPRegenRate();
}

float APS_PlayerState::GetAP() const
{
	return AttributeSet->GetAP();
}

float APS_PlayerState::GetMaxAP() const
{
	return AttributeSet->GetMaxAP();
}

float APS_PlayerState::GetAPRegenRate() const
{
	return AttributeSet->GetAPRegenRate();
}

float APS_PlayerState::GetDamage() const
{
	return AttributeSet->GetDamage();
}

float APS_PlayerState::GetDeffense() const
{
	return AttributeSet->GetDeffense();
}

float APS_PlayerState::GetStrength() const
{
	return AttributeSet->GetStrength();
}

float APS_PlayerState::GetAgility() const
{
	return AttributeSet->GetAgility();
}

float APS_PlayerState::GetIntelligence() const
{
	return AttributeSet->GetIntelligence();
}

float APS_PlayerState::GetLuck() const
{
	return AttributeSet->GetLuck();
}


float APS_PlayerState::GetMoveSpeed() const
{
	return AttributeSet->GetMoveSpeed();
}

int32 APS_PlayerState::GetCharacterLevel() const
{
	return AttributeSet->GetCharacterLevel();
}

int32 APS_PlayerState::GetEXP() const
{
	return AttributeSet->GetEXP();
}

int32 APS_PlayerState::GetEXPBounty() const
{
	return AttributeSet->GetEXPBounty();
}

int32 APS_PlayerState::GetGold() const
{
	return AttributeSet->GetGold();
}

int32 APS_PlayerState::GetGoldBounty() const
{
	return AttributeSet->GetGoldBounty();
}

void APS_PlayerState::LoadStat(TArray<FST_Item> EquipItems)
{
	FStat Stat;
	for (int i = 0; i < EquipItems.Num(); i++)
	{
		Stat += EquipItems[i].Stat;
	}
	//AttributeSet->
}

void APS_PlayerState::UseDurationItem(const FST_Item& Item)
{
	//UKismetSystemLibrary::K2_SetTimer(this, "UseItem",);
}

void APS_PlayerState::UseConsumableItem(const FST_Item& Item)
{
	FOnAttributeChangeData Data = FOnAttributeChangeData();
	Data.NewValue = Item.Stat.HP;
	HPChanged(Data);
	Data.NewValue = Item.Stat.MP;
	MPChanged(Data);

}

void APS_PlayerState::HPChanged(const FOnAttributeChangeData& Data)
{
	float HP = Data.NewValue;
	
	AC_Player* Player = Cast<AC_Player>(GetPawn());
	if (Player)
	{
		/*UGDFloatingStatusBarWidget* PlayerFloatingStatusBar = Player->GetFloatingStatusBar();
		if (PlayerFloatingStatusBar)
		{
			PlayerFloatingStatusBar->SetHealthPercentage(Health / GetMaxHealth());
		}*/
	}

	if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	{
		/*if (Player)
			Player->Die();*/
	}
}

void APS_PlayerState::MaxHPChanged(const FOnAttributeChangeData& Data)
{ 
	float MaxHP = Data.NewValue;

	AC_Player* Player = Cast<AC_Player>(GetPawn());
	if (Player)
	{
		/*UGDFloatingStatusBarWidget* PlayerFloatingStatusBar = Player->GetFloatingStatusBar();
		if (PlayerFloatingStatusBar)
		{
			PlayerFloatingStatusBar->SetHealthPercentage(GetHealth() / MaxHealth);
		}*/
	}

	APC_Player* PC = Cast<APC_Player>(GetOwner());
	if (PC)
	{
		/*UGDHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetMaxHealth(MaxHealth);
		}*/
	}
}

void APS_PlayerState::HPRegenRateChanged(const FOnAttributeChangeData& Data)
{
	float HPRegenRate = Data.NewValue;
	APC_Player* PC = Cast<APC_Player>(GetOwner());
	if (PC)
	{
		/*UGDHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetHealthRegenRate(HealthRegenRate);
		}*/
	}
}

void APS_PlayerState::MPChanged(const FOnAttributeChangeData& Data)
{
	float MP = Data.NewValue;
	AC_Player* Player = Cast<AC_Player>(GetPawn());
	if (Player)
	{
		/*UGDFloatingStatusBarWidget* PlayerFloatingStatusBar = Player->GetFloatingStatusBar();
		if (PlayerFloatingStatusBar)
		{
			PlayerFloatingStatusBar->SetManaPercentage(Mana / GetMaxMana());
		}*/
	}
}

void APS_PlayerState::MaxMPChanged(const FOnAttributeChangeData& Data)
{
	float MaxMP = Data.NewValue;

	// Update floating status bar
	AC_Player* Player = Cast<AC_Player>(GetPawn());
	if (Player)
	{
		/*UGDFloatingStatusBarWidget* PlayerFloatingStatusBar = Player->GetFloatingStatusBar();
		if (PlayerFloatingStatusBar)
		{
			PlayerFloatingStatusBar->SetManaPercentage(GetMP() / MaxMP);
		}*/
	}

	// Update the HUD
	APC_Player* PC = Cast<APC_Player>(GetOwner());
	if (PC)
	{
		/*UGDHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetMaxMana(MaxMP);
		}*/
	}
}

void APS_PlayerState::MPRegenRateChanged(const FOnAttributeChangeData& Data)
{
	float MPRegenRate = Data.NewValue;

	// Update the HUD
	APC_Player* PC = Cast<APC_Player>(GetOwner());
	if (PC)
	{
		/*UGDHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetMPRegenRate(MPRegenRate);
		}*/
	}
}

void APS_PlayerState::APChanged(const FOnAttributeChangeData& Data)
{
	float AP = Data.NewValue;
}

void APS_PlayerState::MaxAPChanged(const FOnAttributeChangeData& Data)
{
	float MaxAP = Data.NewValue;

	// Update the HUD
	APC_Player* PC = Cast<APC_Player>(GetOwner());
	if (PC)
	{
		/*UGDHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetMaxAP(MaxAP);
		}*/
	}
}

void APS_PlayerState::APRegenRateChanged(const FOnAttributeChangeData& Data)
{
	float APRegenRate = Data.NewValue;

	// Update the HUD
	APC_Player* PC = Cast<APC_Player>(GetOwner());
	if (PC)
	{
		/*UGDHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetAPRegenRate(APRegenRate);
		}*/
	}
}

void APS_PlayerState::EXPChanged(const FOnAttributeChangeData& Data)
{
	float EXP = Data.NewValue;

	// Update the HUD
	APC_Player* PC = Cast<APC_Player>(GetOwner());
	if (PC)
	{
		/*UGDHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetExperience(EXP);
		}*/
	}
}

void APS_PlayerState::GoldChanged(const FOnAttributeChangeData& Data)
{
	float Gold = Data.NewValue;

	// Update the HUD
	APC_Player* PC = Cast<APC_Player>(GetOwner());
	if (PC)
	{
		/*UGDHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetGold(Gold);
		}*/
	}
}

void APS_PlayerState::CharacterLevelChanged(const FOnAttributeChangeData& Data)
{
	float CharacterLevel = Data.NewValue;

	// Update the HUD
	APC_Player* PC = Cast<APC_Player>(GetOwner());
	if (PC)
	{
		/*UGDHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetPlayerLevel(CharacterLevel);
		}*/
	}
}

void APS_PlayerState::DamageChanged(const FOnAttributeChangeData& Data)
{
	float Damage = Data.NewValue;
}

void APS_PlayerState::DeffenseChanged(const FOnAttributeChangeData& Data)
{
	float Deffense = Data.NewValue;
}

void APS_PlayerState::StrengthChanaged(const FOnAttributeChangeData& Data)
{
	float Strength = Data.NewValue;
}

void APS_PlayerState::AgilityChanged(const FOnAttributeChangeData& Data)
{
	float Agility = Data.NewValue;
}

void APS_PlayerState::IntelligenceChanged(const FOnAttributeChangeData& Data)
{
	float Intelligence = Data.NewValue;
}

void APS_PlayerState::LuckChanged(const FOnAttributeChangeData& Data)
{
	float Luck = Data.NewValue;
}

void APS_PlayerState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
}

