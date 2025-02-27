#pragma once

UENUM(BlueprintType)
enum class EKGUnits : uint8
{
	// Special Units
	E_Digger		UMETA(DisplayName="Digger"),
	E_Thief			UMETA(DisplayName="Thief"),
	E_BoostPicker	UMETA(DisplayName="BoostPicker"),
	E_Rebel			UMETA(DisplayName="Rebel"),		

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

	// Missle
	E_Rocket		UMETA(DisplayName="Rocket"),
};

UENUM(BlueprintType)
enum class EKGBuildings : uint8
{
	// Collectors
	E_Mine				UMETA(DisplayName="Mine"),
	E_Prison			UMETA(DisplayName="Prison"),
	E_Bank				UMETA(DisplayName="Bank"),
	E_UVKSC				UMETA(DisplayName="UVKSC"),
	
	// Ground Units
	E_Microsoft			UMETA(DisplayName="Microsoft"),			//pb: pozor na trademark //jp: pořeším, vymyslíme lepší názvy
	E_Barracs			UMETA(DisplayName="Barracs"),
	E_Garage			UMETA(DisplayName="Garage"),
	E_LPlatform			UMETA(DisplayName="LaunchPlatform"),
	E_Minecraft			UMETA(DisplayName="Minecraft"),			//pb: pozor na trademark //jp: pořeším, vymyslíme lepší názvy
	
	// Aircraft
	E_Heliport			UMETA(DisplayName="Heliport"),
	E_Runway			UMETA(DisplayName="Runway"),
	E_Aerial			UMETA(DisplayName="Aerial"),
	E_Hangar			UMETA(DisplayName="Hangar"),
	
	// Missle
	E_RocketLauncher	UMETA(DisplayName="RocketLauncher"),
};
