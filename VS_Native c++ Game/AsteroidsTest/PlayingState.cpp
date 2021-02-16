#include "PlayingState.h"
#include "System.h"
#include "Graphics.h"
#include "Game.h"
#include "Font.h"

PlayingState::PlayingState():
scoreFont_(0),
score_(0),
lifesFont_(0),
playerLifes(0)
{
}

PlayingState::~PlayingState()
{
}

void PlayingState::OnActivate(System *system, StateArgumentMap &args)
{
	if (scoreFont_ == 0 )
	{
		scoreFont_ = system->GetGraphics()->CreateXFont("Arial", 30);
		lifesFont_ =  system->GetGraphics()->CreateXFont("Arial", 25);
	}
	Game *game = system->GetGame();

	level_ = args["Level"].asInt;
	game->InitialiseLevel(level_);
	
}

void PlayingState::OnUpdate(System *system)
{
	Game *game = system->GetGame();
	game->Update(system);
	score_ = game->score_;
	playerLifes = game->playerlife;
	if (game->IsLevelComplete())
	{
		StateArgumentMap args;
		args["Level"].asInt = level_ + 1;
		system->SetNextState("LevelStart", args);
	}
	else if (game->IsGameOver())
	{
		system->SetNextState("GameOver");
	}
}

void PlayingState::OnRender(System *system)
{
	Game *game = system->GetGame();
	game->RenderEverything(system->GetGraphics());

	char scoreText[256];
	sprintf_s(scoreText, "Score : %d", score_);
	int textWidth = scoreFont_->CalculateTextWidth(scoreText);
	int textX = (800 - textWidth) * 0.1;
	int textY = (600 - 48) * 0.1;
	scoreFont_->DrawText(scoreText, textX, textY, 0xffffff00);

	
	char LivesText[256];
	sprintf_s(LivesText, "Lifes : %d", playerLifes);
	textWidth = lifesFont_->CalculateTextWidth(LivesText);
	textX = (800 - textWidth);
	textY = (600 - 48);
	lifesFont_->DrawText(LivesText, textX, textY, 0xffffff00);


}

void PlayingState::OnDeactivate(System *system)
{
	system->GetGraphics()->DestroyXFont(scoreFont_);
	scoreFont_ = 0;
}
