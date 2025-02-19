#include "ResourcesWidget.h"

#include "Components/TextBlock.h"
#include "Emerald_MRVR/Components/Resources/ResourcesComponent.h"
#include "Emerald_MRVR/CORE/Pawns/VRPawn.h"


void UResourcesWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	AVRPawn* VR_Pawn = Cast<AVRPawn>(GetOwningPlayerPawn());
	if (VR_Pawn && VR_Pawn->ResourcesComponent && VR_Pawn->ResourcesComponent->AvailableResources)
	{
		TXT_Resources->SetText(FText::AsNumber(VR_Pawn->ResourcesComponent->AvailableResources));
	}
}

void UResourcesWidget::UpdateResourcesWidget(float Resources)
{
	TXT_Resources->SetText(FText::AsNumber(Resources));
}

