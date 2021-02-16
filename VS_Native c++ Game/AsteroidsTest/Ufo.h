#ifndef Ufo_H_INCLUDED
#define Ufo_H_INCLUDED

#include "GameEntity.h"
#include "d3dx9.h"
class Ufo : public GameEntity
{
public:
	Ufo(const D3DXVECTOR3 &position,
		const D3DXVECTOR3 &velocity,
		int size);

	void Update(System *system);
	void Render(Graphics *graphics) const;

	D3DXVECTOR3 GetVelocity() const;
	int GetSize() const;
	int ufoHealCount;
private:

	D3DXVECTOR3 velocity_;
	D3DXVECTOR3 axis_;
	float angle_;
	float angularSpeed_;
	int size_;

};

#endif // Ufo_H_INCLUDED
