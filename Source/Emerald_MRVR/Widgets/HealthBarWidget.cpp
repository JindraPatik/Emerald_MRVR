#include "HealthBarWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Emerald_MRVR/Components/HealthComponent.h"
#include "Emerald_MRVR/CORE/MR_General.h"


void UHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AMR_General* MR_General = Cast<AMR_General>(GetOwningPlayerPawn());
    	
	if (MR_General && MR_General->ResourcesComponent && MR_General->HealthComponent)
	{
		float Health = MR_General->HealthComponent->Health;
		TXT_Health->SetText(FText::AsNumber(Health));
		float Percent = Health / (MR_General->HealthComponent->MaxHealth); 
		HealthProgressBar->SetPercent(Percent);
	}
}

void UHealthBarWidget::UpdateHealthWidget(float NewHealth)
{
	AMR_General* MR_General = Cast<AMR_General>(GetOwningPlayerPawn()); // tady bude chyba???
    	
    	if (MR_General && MR_General->HealthComponent)
    	{
    		float Health = NewHealth;
    		TXT_Health->SetText(FText::AsNumber(Health));
    		float Percent = NewHealth / (MR_General->HealthComponent->MaxHealth); 
    		HealthProgressBar->SetPercent(Percent);
    	}
}
