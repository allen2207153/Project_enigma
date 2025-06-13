#include "WBP_TitleScreen.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"

void UWBP_TitleScreen::NativeConstruct()
{
	Super::NativeConstruct();

	if (Hinttext) Hinttext->SetVisibility(ESlateVisibility::Visible);
	if (playbutton) playbutton->SetVisibility(ESlateVisibility::Hidden);
	if (optionbutton) optionbutton->SetVisibility(ESlateVisibility::Hidden);
	if (quitbutton) quitbutton->SetVisibility(ESlateVisibility::Hidden);

	SetupButtonEvents();
}

void UWBP_TitleScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bMenuShown && Hinttext)
	{
		// 呼吸のようなフェードイン・アウト（RenderOpacity）
		BreathTime += InDeltaTime;
		float Alpha = (FMath::Sin(BreathTime * 2.0f) + 1.0f) * 0.5f; // 0〜1
		Hinttext->SetRenderOpacity(Alpha);
	}

	// Aボタンを押されたらメニュー表示（Gamepad用）
	if (!bMenuShown)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC && PC->IsInputKeyDown(EKeys::Gamepad_FaceButton_Bottom))
		{
			ShowMenu();
			bMenuShown = true;
		}
	}
}

void UWBP_TitleScreen::ShowMenu()
{
	if (Hinttext) Hinttext->SetVisibility(ESlateVisibility::Hidden);
	if (playbutton) playbutton->SetVisibility(ESlateVisibility::Visible);
	if (optionbutton) optionbutton->SetVisibility(ESlateVisibility::Visible);
	if (quitbutton) quitbutton->SetVisibility(ESlateVisibility::Visible);

	playbutton->SetKeyboardFocus();
}

void UWBP_TitleScreen::SetupButtonEvents()
{
	if (playbutton)
	{
		playbutton->OnClicked.AddDynamic(this, &UWBP_TitleScreen::OnPlayClicked);
	}
	if (quitbutton)
	{
		quitbutton->OnClicked.AddDynamic(this, &UWBP_TitleScreen::OnQuitClicked);
	}
}

void UWBP_TitleScreen::OnPlayClicked()
{
	if (!LevelToLoad.IsNone())
	{
		UGameplayStatics::OpenLevel(this, LevelToLoad);
	}
}

void UWBP_TitleScreen::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
