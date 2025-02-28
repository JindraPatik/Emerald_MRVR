#pragma once

UENUM(BlueprintType)
enum class EKGUnits : uint8
{
	// Special Units
	E_Digger		UMETA(DisplayName="Digger"),
	E_Thief			UMETA(DisplayName="Thief"),
	E_Picker		UMETA(DisplayName="Picker"),
	E_Rebel			UMETA(DisplayName="Rebel"),		
	E_Rocket		UMETA(DisplayName="Rocket"),

	// Ground Units
	E_Scout			UMETA(DisplayName="Scout"),	
	E_Vanguard		UMETA(DisplayName="Vanguard"),
	E_Tank			UMETA(DisplayName="Tank"),
	E_Striker		UMETA(DisplayName="Striker"),
	E_Blaster		UMETA(DisplayName="Blaster"),

	// Flying Units
	E_Helicopter	UMETA(DisplayName="Helicopter"),
	E_Jet			UMETA(DisplayName="Jet"),
	E_Zeppelin		UMETA(DisplayName="Zeppelin"),
	E_Bomber		UMETA(DisplayName="Bomber"),
};

UENUM(BlueprintType)
enum class EKGBuildings : uint8
{
	// Special
	E_Mine				UMETA(DisplayName="Mine"),
	E_Prison			UMETA(DisplayName="Prison"),
	E_Vault				UMETA(DisplayName="Vault"),
	E_Pit				UMETA(DisplayName="Pit"),
	E_Silo				UMETA(DisplayName="Silo"),
	
	// Ground Units
	E_Outpost			UMETA(DisplayName="Outpost"),			
	E_Fort				UMETA(DisplayName="Fort"),
	E_Bastion			UMETA(DisplayName="Bastion"),
	E_Platform			UMETA(DisplayName="Platform"),
	E_Depot				UMETA(DisplayName="Depot"),			
	
	// Aircraft
	E_Helipad			UMETA(DisplayName="Helipad"),
	E_Runway			UMETA(DisplayName="Runway"),
	E_Aerial			UMETA(DisplayName="Aerial"),
	E_Hangar			UMETA(DisplayName="Hangar"),
	
};
