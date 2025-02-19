#include "HealthBarWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Emerald_MRVR/Components/Health/HealthComponent.h"
#include "Emerald_MRVR/CORE/Pawns/VRPawn.h"


void UHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AVRPawn* VR_Pawn = Cast<AVRPawn>(GetOwningPlayerPawn());
    	
	if (VR_Pawn && VR_Pawn->ResourcesComponent && VR_Pawn->HealthComponent)
	{
		float Health = VR_Pawn->HealthComponent->Health;
		TXT_Health->SetText(FText::AsNumber(Health));
		float Percent = Health / (VR_Pawn->HealthComponent->MaxHealth); 
		HealthProgressBar->SetPercent(Percent);
	}
}

void UHealthBarWidget::UpdateHealthWidget(float NewHealth)
{
	AVRPawn* VR_Pawn = Cast<AVRPawn>(GetOwningPlayerPawn());
	
    	if (VR_Pawn && VR_Pawn->HealthComponent)
    	{
    		float Health = NewHealth;
    		TXT_Health->SetText(FText::AsNumber(Health));
    		float Percent = NewHealth / (VR_Pawn->HealthComponent->MaxHealth); 
    		HealthProgressBar->SetPercent(Percent);
    	}
}
