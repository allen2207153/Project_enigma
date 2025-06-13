#include "GameMode_TitleScreen.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "WBP_TitleScreen.h"
#include "Components/Button.h"

void AGameMode_TitleScreen::BeginPlay()
{
	Super::BeginPlay();

	if (!TitleWidgetClass) return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC) return;

    UUserWidget* CreatedWidget = CreateWidget<UUserWidget>(PC, TitleWidgetClass);
    TitleWidget = Cast<UWBP_TitleScreen>(CreatedWidget);
    if (TitleWidget)
    {
        TitleWidget->AddToViewport();

        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(TitleWidget->GetCachedWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputMode.SetHideCursorDuringCapture(false);
        PC->SetInputMode(InputMode);
        PC->SetShowMouseCursor(true);
    }
}