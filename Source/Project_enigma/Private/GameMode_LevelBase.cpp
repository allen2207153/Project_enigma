#include "GameMode_LevelBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include <CharacterControllerBase.h>

AGameMode_LevelBase::AGameMode_LevelBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bGameEnded = false;
}

void AGameMode_LevelBase::BeginPlay()
{
	Super::BeginPlay();
	LevelStartTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
}

void AGameMode_LevelBase::OnLevelClear()
{
    if (bGameEnded) return;
    bGameEnded = true;

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        ACharacterControllerBase* MyController = Cast<ACharacterControllerBase>(PC);
        if (MyController && MyController->LevelUIInstance)
        {
            MyController->LevelUIInstance->SetResultData(CoinsCollected, DiamondsCollected, GetElapsedTime());
            MyController->LevelUIInstance->ShowResultScreen();
        }
    }
}

void AGameMode_LevelBase::OnGameOver()
{
	if (bGameEnded) return;
	bGameEnded = true;

	// 例：UI GameOver 画面を表示
	UE_LOG(LogTemp, Warning, TEXT("💀 Game Over! Time: %.2f sec"), GetElapsedTime());
}

void AGameMode_LevelBase::AddCoin(int32 Amount)
{
	CoinsCollected += Amount;
}

void AGameMode_LevelBase::AddDiamond(int32 Amount)
{
	DiamondsCollected += Amount;
}

float AGameMode_LevelBase::GetElapsedTime() const
{
	return UGameplayStatics::GetRealTimeSeconds(GetWorld()) - LevelStartTime;
}