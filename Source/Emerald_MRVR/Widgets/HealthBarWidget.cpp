#include "HealthBarWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Emerald_MRVR/Components/HealthComponent.h"
#include "Emerald_MRVR/Components/MilitaryBaseComp.h"
#include "Emerald_MRVR/CORE/MR_General.h"


void UHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AMR_General* General = Cast<AMR_General>(GetOwningPlayerPawn());
    	
	if (General && General->ResourcesComponent && General->HealthComponent)
	{
		float Health = General->HealthComponent->Health;
		TXT_Health->SetText(FText::AsNumber(Health));
		float Percent = Health / (General->HealthComponent->MaxHealth); 
		HealthProgressBar->SetPercent(Percent);
	}
}

void UHealthBarWidget::UpdateHealthWidget(float NewHealth)
{
	AMR_General* General = Cast<AMR_General>(GetOwningPlayerPawn());
	
    	if (General && General->HealthComponent)
    	{
    		float Health = NewHealth;
    		TXT_Health->SetText(FText::AsNumber(Health));
    		float Percent = NewHealth / (General->HealthComponent->MaxHealth); 
    		HealthProgressBar->SetPercent(Percent);
    	}
}
