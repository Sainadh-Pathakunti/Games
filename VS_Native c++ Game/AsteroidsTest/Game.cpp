#include "Game.h"
#include "System.h"
#include "OrthoCamera.h"
#include "Background.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Explosion.h"
#include "Keyboard.h"
#include "Random.h"
#include "Maths.h"
#include "Bullet.h"
#include "Font.h"
#include "Ufo.h"
#include "Collision.h"
#include <algorithm>

Game::Game() :
	camera_(0),
	background_(0),
	player_(0),
	ufo_(0),
	collision_(0),
	score_(0),
	playerlife(3)
{
	camera_ = new OrthoCamera();
	camera_->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	camera_->SetFrustum(800.0f, 600.0f, -100.0f, 100.0f);
	background_ = new Background(800.0f, 600.0f);
	collision_ = new Collision();
}

Game::~Game()
{
	delete camera_;
	delete background_;
	delete player_;
	delete ufo_;
	DeleteAllBullets();
	DeleteAllAsteroids();
	DeleteAllExplosions();
	delete collision_;
}

void Game::Update(System *system)
{
	UpdatePlayer(system);
	UpdateUfo(system);
	UpdateAsteroids(system);
	UpdateBullet(system);
	UpdateCollisions();
}

void Game::RenderBackgroundOnly(Graphics *graphics)
{
	camera_->SetAsView(graphics);
	background_->Render(graphics);
}

void Game::RenderEverything(Graphics *graphics)
{
	camera_->SetAsView(graphics);

	background_->Render(graphics);

	if (player_)
	{
		player_->Render(graphics);
	}

	if (ufo_)
	{
		ufo_->Render(graphics);
	}

	for (AsteroidList::const_iterator asteroidIt = asteroids_.begin(),
		end = asteroids_.end();
		asteroidIt != end;
		++asteroidIt)
	{
		(*asteroidIt)->Render(graphics);
	}

	for (BulletList::const_iterator bulletIt = bullets_.begin(),
		end = bullets_.end();
		bulletIt != end;
		++bulletIt)
	{
		(*bulletIt)->Render(graphics);
	}

	

	for (ExplosionList::const_iterator explosionIt = explosions_.begin(),
		end = explosions_.end();
		explosionIt != end;
		++explosionIt)
	{
		(*explosionIt)->Render(graphics);
	}

}

void Game::InitialiseLevel(int numAsteroids)
{
	DeleteAllAsteroids();
	DeleteAllExplosions();
	DeleteAllBullets();
	SpawnPlayer();
	if(numAsteroids % 2 == 0){
		SpawnUfo();
	}
	SpawnAsteroids(numAsteroids);
}

bool Game::IsLevelComplete() const
{
	return (asteroids_.empty() && explosions_.empty());
}

bool Game::IsGameOver() const
{
	return (player_ == 0 && explosions_.empty());
}

void Game::DoCollision(GameEntity *a, GameEntity *b)
{
	Ship *player = static_cast<Ship *>(a == player_ ? a : (b == player_ ? b : 0));
	Ufo *ufo = static_cast<Ufo *>(a == ufo_ ? a : (b == ufo_ ? b : 0));
	Bullet *bullet = static_cast<Bullet *>(IsBullet(a) ? a : (IsBullet(b) ? b : 0));
	Asteroid *asteroid = static_cast<Asteroid *>(IsAsteroid(a) ? a : (IsAsteroid(b) ? b : 0));

	if (player && asteroid)
	{
		if(playerlife == 0){
		score_ = 0;
		playerlife = 3;
		AsteroidHit(asteroid);
		DeletePlayer();
		return;
		}
		playerlife--;
		player_->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		
		
	}

	if (player && ufo)
	{
		if(playerlife == 1){
		score_ = 0;
		playerlife = 3;
		UfoHit();
		DeletePlayer();
		return;
		}
		playerlife--;
		player_->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		
		
	}

	if (bullet && asteroid)
	{
		score_++;
		AsteroidHit(asteroid);
		DeleteBullet(bullet);
	}
	if (bullet && ufo)
	{
		score_ += 5; 
		ufo_->ufoHealCount++;
		if(ufo_->ufoHealCount == 5){
			UfoHit();
		}
		DeleteBullet(bullet);
	}
}



void Game::SpawnPlayer()
{
	DeletePlayer();
	player_ = new Ship();
	player_->EnableCollisions(collision_, 10.0f);
}

void Game::DeletePlayer()
{
	delete player_;
	player_ = 0;
}

void Game::UpdatePlayer(System *system)
{
	if (player_ == 0)
		return;

	Keyboard *keyboard = system->GetKeyboard();

	float acceleration = 0.0f;
	if (keyboard->IsKeyHeld(VK_UP) || keyboard->IsKeyHeld('W'))
	{
		acceleration = 1.0f;
	}
	else if (keyboard->IsKeyHeld(VK_DOWN) || keyboard->IsKeyHeld('S'))
	{
		acceleration = -1.0f;
	}

	float rotation = 0.0f;
	if (keyboard->IsKeyHeld(VK_RIGHT) || keyboard->IsKeyHeld('D'))
	{
		rotation = -1.0f;
	}
	else if (keyboard->IsKeyHeld(VK_LEFT) || keyboard->IsKeyHeld('A'))
	{
		rotation = 1.0f;
	}

	player_->SetControlInput(acceleration, rotation);
	player_->Update(system);
	WrapEntity(player_);

	if (keyboard->IsKeyPressed(VK_SPACE))
	{
		D3DXVECTOR3 playerForward = player_->GetForwardVector();
		D3DXVECTOR3 bulletPosition = player_->GetPosition() + playerForward * 10.0f;
		SpawnBullet(bulletPosition, playerForward);
	}
}

void Game::UpdateAsteroids(System *system)
{
	for (AsteroidList::const_iterator asteroidIt = asteroids_.begin(),
		end = asteroids_.end();
		asteroidIt != end;
	++asteroidIt)
	{
		(*asteroidIt)->Update(system);
		WrapEntity(*asteroidIt);
	}
}

void Game::UpdateUfo(System *system)
{
	if(ufo_)
	{
		(ufo_)->Update(system);
		WrapEntity(ufo_);
	}
}


void Game::UpdateBullet(System *system)
{
	for (BulletList::const_iterator bulletIt = bullets_.begin(),
		end = bullets_.end();
		bulletIt != end;
	++bulletIt)
	{
		(*bulletIt)->Update(system);
		WrapEntity(*bulletIt);
	}

}

void Game::WrapEntity(GameEntity *entity) const
{
	D3DXVECTOR3 entityPosition = entity->GetPosition();
	entityPosition.x = Maths::WrapModulo(entityPosition.x, -400.0f, 400.0f);
	entityPosition.y = Maths::WrapModulo(entityPosition.y, -300.0f, 300.0f);
	entity->SetPosition(entityPosition);
}

void Game::DeleteAllAsteroids()
{
	for (AsteroidList::const_iterator asteroidIt = asteroids_.begin(),
		end = asteroids_.end();
		asteroidIt != end;
		++asteroidIt)
	{
		delete (*asteroidIt);
	}

	asteroids_.clear();
}


void Game::DeleteAllBullets()
{
	for (BulletList::const_iterator bulletIt = bullets_.begin(),
		end = bullets_.end();
		bulletIt != end;
		++bulletIt)
	{
		delete (*bulletIt);
	}

	bullets_.clear();
}

void Game::DeleteAllExplosions()
{
	for (ExplosionList::const_iterator explosionIt = explosions_.begin(),
		end = explosions_.end();
		explosionIt != end;
	++explosionIt)
	{
		delete (*explosionIt);
	}

	explosions_.clear();
}

void Game::SpawnBullet(const D3DXVECTOR3 &position,
	const D3DXVECTOR3 &direction)
{
	//DeleteBullet();

	printf("the postion is X: %f  y : %f z : %f\n", position.x,position.y,position.z);
	Bullet *bullet_ = new Bullet(position, direction);
	bullet_->EnableCollisions(collision_, 3.0f);
	bullets_.push_back(bullet_);
}

void Game::DeleteBullet(Bullet *bullet)
{
	bullets_.remove(bullet);
	delete bullet;
}

void Game::SpawnAsteroids(int numAsteroids)
{
	float halfWidth = 800.0f * 0.5f;
	float halfHeight = 600.0f * 0.5f;
	for (int i = 0; i < numAsteroids; i++)
	{
		float x = Random::GetFloat(-halfWidth, halfWidth);
		float y = Random::GetFloat(-halfHeight, halfHeight);
		D3DXVECTOR3 position = D3DXVECTOR3(x, y, 0.0f);
		SpawnAsteroidAt(position, 3);
	}
}

void Game::SpawnAsteroidAt(const D3DXVECTOR3 &position, int size)
{
	const float MAX_ASTEROID_SPEED = 1.0f;

	float angle = Random::GetFloat(Maths::TWO_PI);
	D3DXMATRIX randomRotation;
	D3DXMatrixRotationZ(&randomRotation, angle);
	D3DXVECTOR3 velocity = D3DXVECTOR3(0.0f, Random::GetFloat(MAX_ASTEROID_SPEED), 0.0f);
	D3DXVec3TransformNormal(&velocity, &velocity, &randomRotation);

	Asteroid *asteroid = new Asteroid(position, velocity, size);
	asteroid->EnableCollisions(collision_, size * 5.0f);
	asteroids_.push_back(asteroid);
}


void Game::SpawnUfo()
{
	float halfWidth = 800.0f * 0.5f;
	float halfHeight = 600.0f * 0.5f;
		float x = Random::GetFloat(-halfWidth, halfWidth);
		float y = Random::GetFloat(-halfHeight, halfHeight);
		D3DXVECTOR3 position = D3DXVECTOR3(x, y, 0.0f);
		SpawnUfoAt(position, 3);
	
}

void Game::SpawnUfoAt(const D3DXVECTOR3 &position, int size)
{
	const float MAX_UFO_SPEED = 2.0f;

	float angle = Random::GetFloat(Maths::TWO_PI);
	D3DXMATRIX randomRotation;
	D3DXMatrixRotationZ(&randomRotation, angle);
	D3DXVECTOR3 velocity = D3DXVECTOR3(0.0f, Random::GetFloat(MAX_UFO_SPEED), 0.0f);
	D3DXVec3TransformNormal(&velocity, &velocity, &randomRotation);

	ufo_ = new Ufo(position, velocity, size);
	ufo_->EnableCollisions(collision_, size * 5.0f);

}

bool Game::IsAsteroid(GameEntity *entity) const
{
	return (std::find(asteroids_.begin(),
		asteroids_.end(), entity) != asteroids_.end()); 
}

bool Game::IsBullet(GameEntity *entity) const
{
	return (std::find(bullets_.begin(),
		bullets_.end(), entity) != bullets_.end()); 
}

void Game::AsteroidHit(Asteroid *asteroid)
{
	int oldSize = asteroid->GetSize();
	if (oldSize > 1)
	{
		int smallerSize = oldSize -1;
		D3DXVECTOR3 position = asteroid->GetPosition();
		SpawnAsteroidAt(position, smallerSize);
		SpawnAsteroidAt(position, smallerSize);
	}
	DeleteAsteroid(asteroid);
}

void Game::DeleteAsteroid(Asteroid *asteroid)
{
	asteroids_.remove(asteroid);
	delete asteroid;
}

void Game::UfoHit()
{
	DeleteUfo();
}

void Game::DeleteUfo()
{
	delete ufo_;
	ufo_ = 0;
}

void Game::UpdateCollisions()
{
	collision_->DoCollisions(this);
}
