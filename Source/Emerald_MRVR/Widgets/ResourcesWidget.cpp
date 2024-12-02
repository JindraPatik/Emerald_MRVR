#include "ResourcesWidget.h"

#include "Components/TextBlock.h"
#include "Emerald_MRVR/Components/ResourcesComponent.h"
#include "Emerald_MRVR/CORE/MR_General.h"


void UResourcesWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	AMR_General* MR_General = Cast<AMR_General>(GetOwningPlayerPawn());
	if (MR_General && MR_General->ResourcesComponent && MR_General->ResourcesComponent->AvailableResources)
	{
		TXT_Resources->SetText(FText::AsNumber(MR_General->ResourcesComponent->AvailableResources));
	}
}

void UResourcesWidget::UpdateResourcesWidget(float Resources)
{
	TXT_Resources->SetText(FText::AsNumber(Resources));
}

