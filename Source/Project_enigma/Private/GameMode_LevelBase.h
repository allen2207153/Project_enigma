
#pragma once

#include "CoreMinimal.h"
#include <WBP_LevelUIBase.h>
#include "GameFramework/GameModeBase.h"
#include "GameMode_LevelBase.generated.h"


UCLASS()

class AGameMode_LevelBase : public AGameModeBase

{
	GENERATED_BODY()

public:
	AGameMode_LevelBase();

protected:
	virtual void BeginPlay() override;

private:
	float LevelStartTime;
	bool bGameEnded;

public:
	// === ゲーム統計 ===
	UPROPERTY(BlueprintReadOnly, Category = "GameStats")
	int32 CoinsCollected = 0;

	UPROPERTY(BlueprintReadOnly, Category = "GameStats")
	int32 DiamondsCollected = 0;

	// === UI 參考（BeginPlay 時從 Controller 取得）===
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWBP_LevelUIBase> LevelUI;

	// === ゲームの進行処理 ===
	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void AddCoin(int32 Amount = 1);

	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void AddDiamond(int32 Amount = 1);

	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void OnLevelClear();

	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void OnGameOver();

	UFUNCTION(BlueprintCallable, Category = "GameStats")
	int32 GetCoins() const { return CoinsCollected; }

	UFUNCTION(BlueprintCallable, Category = "GameStats")
	int32 GetDiamonds() const { return DiamondsCollected; }

	UFUNCTION(BlueprintCallable)
	float GetElapsedTime() const;
};