#pragma once

#include <image.h>
#include <model.h>


//////////////////////////////////////////////////////////////////////////////
//
// ExplosionGeo header
//
//////////////////////////////////////////////////////////////////////////////

class ExplosionGeo : public Geo {
protected:
	ExplosionGeo(Number* ptime) :
		Geo(ptime)
	{
	}

public:
	virtual void AddExplosion(
		const Vector& vecPosition,
		const Vector& vecForward,
		const Vector& vecRight,
		const Vector& vecVelocity,
		float radiusExplosion,
        float radiusShockWave,
		const Color& color,
		int   countDecals,
        TVector<TRef<AnimatedImage> > vimage,
        Image* pimageShockWave
	) = 0;
};

TRef<ExplosionGeo> CreateExplosionGeo(Number* ptime);