#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <d3dx9math.h>
#include <list>

class OrthoCamera;
class Background;
class Ship;
class Bullet;
class Asteroid;
class Ufo;
class Explosion;
class Collision;
class System;
class Graphics;
class GameEntity;
class Font;
class Game
{
public:
	Game();
	~Game();

	void Update(System *system);
	void RenderBackgroundOnly(Graphics *graphics);
	void RenderEverything(Graphics *graphics);

	void InitialiseLevel(int numAsteroids);
	bool IsLevelComplete() const;
	bool IsGameOver() const;
	int score_;
	int playerlife;
	void DoCollision(GameEntity *a, GameEntity *b);

private:
	Game(const Game &);
	void operator=(const Game &);

	typedef std::list<Asteroid *> AsteroidList;
	typedef std::list<Explosion *> ExplosionList;
	typedef std::list<Bullet *> BulletList;

	void SpawnPlayer();
	void DeletePlayer();

	void SpawnUfo();
	
	void UpdatePlayer(System *system);
	void UpdateAsteroids(System *system);
	void UpdateBullet(System *system);
	void UpdateUfo(System *system);
	void WrapEntity(GameEntity *entity) const;

	void DeleteAllAsteroids();
	void DeleteAllExplosions();
	void DeleteAllBullets();
	
	void SpawnBullet(const D3DXVECTOR3 &position,
		const D3DXVECTOR3 &direction);
	void DeleteBullet(Bullet *bullet);


	void SpawnUfoAt(const D3DXVECTOR3 &position, int size);
	
	void SpawnAsteroids(int numAsteroids);
	void SpawnAsteroidAt(const D3DXVECTOR3 &position, int size);
	
	bool IsAsteroid(GameEntity *entity) const;
	bool IsBullet(GameEntity *entity) const;
	void AsteroidHit(Asteroid *asteroid);
	void UfoHit();
	void DeleteAsteroid(Asteroid *asteroid);
	void DeleteUfo();

	void UpdateCollisions();

	OrthoCamera *camera_;

	Background *background_;
	Ship *player_;
	Ufo *ufo_;
	
	AsteroidList asteroids_;
	ExplosionList explosions_;
	BulletList bullets_;
	Collision *collision_;
};

#endif // GAME_H_INCLUDED
