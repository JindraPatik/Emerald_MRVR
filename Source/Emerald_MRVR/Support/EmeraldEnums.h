#pragma once

UENUM(BlueprintType)
enum class EKGUnits : uint8
{
	// Collectors
	E_Harvester		UMETA(DisplayName="Harvester"),
	E_Thief			UMETA(DisplayName="Thief"),
	E_BoostPicker	UMETA(DisplayName="BoostPicker"),
	E_Collaborator	UMETA(DisplayName="Colaborant"),		

	// Ground Units
	E_Voyager		UMETA(DisplayName="Explorel"),	
	E_Brigade		UMETA(DisplayName="Brigade"),
	E_Tank			UMETA(DisplayName="Tank"),
	E_ATACSM		UMETA(DisplayName="ATACSM"),
	E_Mortar		UMETA(DisplayName="Mortar"),

	// Aircraft
	E_Helicopter	UMETA(DisplayName="Helicopter"),
	E_Jet			UMETA(DisplayName="Jet"),
	E_Zeppelin		UMETA(DisplayName="Zeppelin"),
	E_B52			UMETA(DisplayName="B52"),

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
	E_Microsoft			UMETA(DisplayName="Microsoft"),			//pb: pozor na trademark
	E_Barracs			UMETA(DisplayName="Barracs"),
	E_Garage			UMETA(DisplayName="Garage"),
	E_LPlatform			UMETA(DisplayName="LaunchPlatform"),
	E_Minecraft			UMETA(DisplayName="Minecraft"),			//pb: pozor na trademark
	
	// Aircraft
	E_Heliport			UMETA(DisplayName="Heliport"),
	E_Runway			UMETA(DisplayName="Runway"),
	E_Aerial			UMETA(DisplayName="Aerial"),
	E_Hangar			UMETA(DisplayName="Hangar"),
	
	// Missle
	E_RocketLauncher	UMETA(DisplayName="RocketLauncher"),
};
