#include "Collision.h"
#include "Collider.h"
#include "Game.h"
#include <functional>

Collision::Collision()
{
}

Collision::~Collision()
{
	for (int colliderIt = 0, end = colliders_.size() - 1;
		colliderIt != end;
		++colliderIt)
	{
		delete colliders_.at(colliderIt);
	}
}

Collider *Collision::CreateCollider(GameEntity *entity)
{
	Collider *collider = new Collider();

	collider->position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	collider->radius = 0.0f;
	collider->entity = entity;
	collider->enabled = true;
	colliders_.push_back(collider);

	return collider;
}

void Collision::DestroyCollider(Collider *collider)
{
	delete collider;
}

void Collision::UpdateColliderPosition(Collider *collider, const D3DXVECTOR3 &position)
{
	collider->position = position;
}

void Collision::UpdateColliderRadius(Collider *collider, float radius)
{
	collider->radius = radius;
}

void Collision::EnableCollider(Collider *collider)
{
	collider->enabled = true;
}

void Collision::DisableCollider(Collider *collider)
{
	collider->enabled = false;
}

void Collision::DoCollisions(Game *game) const
{
	for (int colliderAIt = 0, end = colliders_.size() - 1;
		colliderAIt != end;
		++colliderAIt)
	{
		int colliderBIt = colliderAIt;
		for (++colliderBIt; colliderBIt != end; ++colliderBIt)
		{
			Collider *colliderA =   (Collider*)colliders_.at(colliderAIt);
			Collider *colliderB =  (Collider*)colliders_.at(colliderBIt);
			if (CollisionTest(colliderA, colliderB))
			{
				game->DoCollision(colliderA->entity, colliderB->entity);
			}
		}
	}
}

bool Collision::CollisionTest(Collider *a, Collider *b)
{
	if (a->enabled == false)
		return false;
	if (b->enabled == false)
		return false;

	D3DXVECTOR3 diff;
	D3DXVec3Subtract(&diff, &a->position, &b->position);
	float distance = D3DXVec3Length(&diff);
	if (distance < (a->radius + b->radius))
	{
		return true;
	}

	return false;
}
