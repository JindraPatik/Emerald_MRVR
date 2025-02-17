#include "ResourcesWidget.h"

#include "Components/TextBlock.h"
#include "Emerald_MRVR/Components/ResourcesComponent.h"
#include "Emerald_MRVR/CORE/VRPawn.h"


void UResourcesWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	AVRPawn* MR_General = Cast<AVRPawn>(GetOwningPlayerPawn());
	if (MR_General && MR_General->ResourcesComponent && MR_General->ResourcesComponent->AvailableResources)
	{
		TXT_Resources->SetText(FText::AsNumber(MR_General->ResourcesComponent->AvailableResources));
	}
}

void UResourcesWidget::UpdateResourcesWidget(float Resources)
{
	TXT_Resources->SetText(FText::AsNumber(Resources));
}

