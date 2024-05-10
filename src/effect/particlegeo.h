#pragma once

#include <model.h>
#include <value.h>
#include <vector.h>


//////////////////////////////////////////////////////////////////////////////
//
// ParticleGeo
//
//////////////////////////////////////////////////////////////////////////////

class Context;
class ParticleData;

class ParticleBehavior
{
    public:
        virtual void            Render (Context* pContext, ParticleData* pParticleData, float fElapsedTime) = 0;
};

class ParticleData
{
    friend  class ParticleGeoImpl;

    public:
        Vector                  m_vecPosition;
        Vector                  m_vecVelocity;
        Vector                  m_vecAcceleration;
        float                   m_fSize;
        float                   m_fDuration;
        float                   m_fAngle;

    public:
        /* void */              ParticleData (void);

    private:
        float                   m_fStartTime;
        ParticleBehavior*       m_pParticleBehavior;

    private:
        void                    InitParticle (float fTime, ParticleBehavior* pParticleBehavior);
};

class ParticleGeo : public Geo 
{
    public:
        /* void */              ParticleGeo (Number* pTime) : Geo (pTime) {}

        virtual ParticleData*   AddAfterburner (void) = 0;
        virtual ParticleData*   AddDamage (void) = 0;
        virtual ParticleData*   AddSmoke (void) = 0;
        virtual ParticleData*   AddFire (void) = 0;
        virtual ParticleData*   AddSpark (void) = 0;
};

TRef<ParticleGeo> CreateParticleGeo (Modeler* pModeler, Number* pTime);

//////////////////////////////////////////////////////////////////////////////