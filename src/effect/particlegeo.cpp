#include "particlegeo.h"
#include "thinggeo.h"

#include <image.h>

//////////////////////////////////////////////////////////////////////////////
//
// Art names
//
//////////////////////////////////////////////////////////////////////////////

#define AWF_EFFECT_AFTERBURNER_SMOKE            "e_boosterbmp"
#define AWF_EFFECT_FIRE                         "e_firebmp"
#define AWF_EFFECT_SMOKE                        "e_smokebmp"
#define AWF_EFFECT_DAMAGE                       "e_damagebmp"
#define AWF_EFFECT_SPARK                        "e_telebmp"

//////////////////////////////////////////////////////////////////////////////
//
// Particle Behaviors
//
//////////////////////////////////////////////////////////////////////////////

class SmokeBehavior : public ParticleBehavior
{
    private:
        TRef<Image>     m_pImageSmoke;

    public:
        //////////////////////////////////////////////////////////////////////
        /* void */      SmokeBehavior (Modeler* pModeler)
        {
            m_pImageSmoke = pModeler->LoadImage (AWF_EFFECT_SMOKE, true);
        }

        //////////////////////////////////////////////////////////////////////
        virtual void    Render (Context* pContext, ParticleData* pParticleData, float fElapsedTime)
        {
            // get the surface we will render from
            TRef<Surface> pSmokeSurface = m_pImageSmoke->GetSurface();

            // compute the remaining duration of the effect
            float   fCountdownToFinish = (pParticleData->m_fDuration - fElapsedTime) / pParticleData->m_fDuration;

            // the effect is grey, but is linearly fading to 0.0 in all components,
            // but green is scaled back a tad to reduce a greenish hue appearing
            Color   color;
            float   fColorComponent = 0.25f * fCountdownToFinish;
            color.SetRGBA (fColorComponent, fColorComponent * 0.8f, fColorComponent, fColorComponent);

            // the effect is growing linearly
            float   fStartSize = 0.75f;
            float   fFinalSize = 3.0f;
            float   fDeltaSize = fFinalSize - fStartSize;
            float   fSize = pParticleData->m_fSize * (fStartSize + (fDeltaSize * (1.0f - fCountdownToFinish)));

            // compute the new location of the effect
            Vector  vecEffectPosition = pParticleData->m_vecPosition + (pParticleData->m_vecVelocity * fElapsedTime) + (pParticleData->m_vecAcceleration * (fElapsedTime * fElapsedTime * 0.5f));

            // draw the decal
            pContext->DrawDecal (pSmokeSurface, color, vecEffectPosition, Vector::GetZero(), Vector::GetZero(), fSize, pParticleData->m_fAngle);
        }

        //////////////////////////////////////////////////////////////////////
};

//////////////////////////////////////////////////////////////////////////////
class FireBehavior : public ParticleBehavior
{
    private:
        TRef<Image>     m_pImageFire;

    public:
        //////////////////////////////////////////////////////////////////////
        /* void */      FireBehavior (Modeler* pModeler)
        {
            m_pImageFire = pModeler->LoadImage (AWF_EFFECT_FIRE, true);
        }

        //////////////////////////////////////////////////////////////////////
        virtual void    Render (Context* pContext, ParticleData* pParticleData, float fElapsedTime)
        {
            // get the surface we will render from
            TRef<Surface> pFireSurface = m_pImageFire->GetSurface();

            // set parameters and compute the durations of the two phases of the effect
            float   fRampUpFraction = 0.1f;
            float   fRampUpDuration = pParticleData->m_fDuration * fRampUpFraction;
            float   fRampDownDuration = pParticleData->m_fDuration - fRampUpDuration;

            // compute the remaining duration of the effect, and clamp it at 1.0 if
            // the effect is still ramping up
            float   fCountdownToFinish = (pParticleData->m_fDuration - fElapsedTime) / fRampDownDuration;
            if (fCountdownToFinish > 1.0f)
                fCountdownToFinish = 1.0f;

            // if the effect hasn't finished
            Color   color;
            float   fSize;

            // Several parts of this effect countdown from 0.5 to 0.0, rather than from
            // 1.0 to 0.0, so we precompute that value for efficiency
            float   fScaledCountdownToFinish = 0.5f * fCountdownToFinish;

            // check to see which phase the effect is in
            if (fElapsedTime < fRampUpDuration)
            {
                // The effect is ramping up, so the puff is white, and it grows from half size
                // to full size during the ramp up period using an exponential growth
                color.SetRGBA (1.0f, fCountdownToFinish, fCountdownToFinish, fScaledCountdownToFinish * fCountdownToFinish);
                fSize = pParticleData->m_fSize * (0.5f + (0.5f * powf (fElapsedTime / fRampUpDuration, 0.666f)));
            }
            else
            {
                // the effect is ramping down, during which it changes from yellow to red, and 
                // shrinks to half size using an exponential decay
                color.SetRGBA (1.0f, fCountdownToFinish, fCountdownToFinish * 0.25f, fScaledCountdownToFinish * fCountdownToFinish);
                fSize = pParticleData->m_fSize * (0.5f + (0.5f * powf (fCountdownToFinish, 0.666f)));
            }

            // compute the new location of the effect
            Vector  vecEffectPosition = pParticleData->m_vecPosition + (pParticleData->m_vecVelocity * fElapsedTime) + (pParticleData->m_vecAcceleration * (fElapsedTime * fElapsedTime * 0.5f));

            // draw the decal
            pContext->DrawDecal (pFireSurface, color, vecEffectPosition, Vector::GetZero(), Vector::GetZero(), fSize, pParticleData->m_fAngle);
        }

        //////////////////////////////////////////////////////////////////////
};

//////////////////////////////////////////////////////////////////////////////
class DamageBehavior : public ParticleBehavior
{
    private:
        TRef<Image>     m_pImageDamage;

    public:
        //////////////////////////////////////////////////////////////////////
        /* void */      DamageBehavior (Modeler* pModeler)
        {
            m_pImageDamage = pModeler->LoadImage (AWF_EFFECT_DAMAGE, true);
        }

        //////////////////////////////////////////////////////////////////////
        virtual void    Render (Context* pContext, ParticleData* pParticleData, float fElapsedTime)
        {
            // we use smoke behavior instead of this
        }

        //////////////////////////////////////////////////////////////////////
};

//////////////////////////////////////////////////////////////////////////////
class AfterburnerBehavior : public ParticleBehavior
{
    private:
        TRef<Image>     m_pImageSmoke;

    public:
        //////////////////////////////////////////////////////////////////////
        /* void */      AfterburnerBehavior (Modeler* pModeler)
        {
            m_pImageSmoke = pModeler->LoadImage (AWF_EFFECT_AFTERBURNER_SMOKE, true);
        }

        //////////////////////////////////////////////////////////////////////
        virtual void    Render (Context* pContext, ParticleData* pParticleData, float fElapsedTime)
        {
            // get the surface we will render from
            TRef<Surface> pSmokeSurface = m_pImageSmoke->GetSurface();

            // set parameters and compute the durations of the two phases of the effect
            float   fRampUpFraction = 0.1f;
            float   fRampUpDuration = pParticleData->m_fDuration * fRampUpFraction;
            float   fRampDownDuration = pParticleData->m_fDuration - fRampUpDuration;

            // compute the remaining duration of the effect, and clamp it at 1.0 if
            // the effect is still ramping up
            float   fCountdownToFinish = (pParticleData->m_fDuration - fElapsedTime) / fRampDownDuration;
            if (fCountdownToFinish > 1.0f)
                fCountdownToFinish = 1.0f;

            // the effect starts yellow and turns grey, but is linearly fading to 0.0 in all components,
            Color   color;
            float   fColorComponent = 0.25f * fCountdownToFinish;
            if (fElapsedTime < fRampUpDuration)
                color.SetRGBA (fColorComponent * 4.0f, fColorComponent * 3.0f, fColorComponent, fColorComponent);
            else
                // green is scaled back a tad to reduce a greenish hue appearing
                color.SetRGBA (fColorComponent, fColorComponent * 0.8f, fColorComponent, fColorComponent);

            // the effect is growing linearly
            float   fStartSize = 1.0f;
            float   fFinalSize = 2.0f;
            float   fDeltaSize = fFinalSize - fStartSize;
            float   fSize = pParticleData->m_fSize * (fStartSize + (fDeltaSize * (1.0f - fCountdownToFinish)));

            // compute the new location of the effect
            Vector  vecEffectPosition = pParticleData->m_vecPosition + (pParticleData->m_vecVelocity * fElapsedTime) + (pParticleData->m_vecAcceleration * (fElapsedTime * fElapsedTime * 0.5f));

            // draw the decal
            pContext->DrawDecal (pSmokeSurface, color, vecEffectPosition, Vector::GetZero(), Vector::GetZero(), fSize, pParticleData->m_fAngle);
        }

        //////////////////////////////////////////////////////////////////////
};

//////////////////////////////////////////////////////////////////////////////
class SparkBehavior : public ParticleBehavior
{
    private:
        TRef<Image>     m_pImageSpark;

    public:
        //////////////////////////////////////////////////////////////////////
        /* void */      SparkBehavior (Modeler* pModeler)
        {
            m_pImageSpark = pModeler->LoadImage (AWF_EFFECT_SPARK, true);
        }

        //////////////////////////////////////////////////////////////////////
        virtual void    Render (Context* pContext, ParticleData* pParticleData, float fElapsedTime)
        {
            // get the surface we will render from
            TRef<Surface> pSparkSurface = m_pImageSpark->GetSurface();

            // draw multiple copies of the effect to produce a blur effect
            for (int i = 0; i < 1; i++)
            {
                // compute the remaining duration of the effect
                float   fCountdownToFinish = (pParticleData->m_fDuration - fElapsedTime) / pParticleData->m_fDuration;

                // the effect is hot blue, fading out
                Color   color;
                float   fColorComponent = 0.5f * fCountdownToFinish * fCountdownToFinish;
                color.SetRGBA (fColorComponent * 1.5f, fColorComponent * 2.0f, fCountdownToFinish, fColorComponent);

                // compute the new location of the effect
                Vector  vecEffectPosition = pParticleData->m_vecPosition + (pParticleData->m_vecVelocity * fElapsedTime) + (pParticleData->m_vecAcceleration * (fElapsedTime * fElapsedTime * 0.5f));

                // draw the decal
                pContext->DrawDecal (pSparkSurface, color, vecEffectPosition, Vector::GetZero(), Vector::GetZero(), pParticleData->m_fSize, pParticleData->m_fAngle + (fCountdownToFinish * pi));

                // scale the elapsed time to produce a blur effect
                fElapsedTime *= 0.97f;
            }
        }

        //////////////////////////////////////////////////////////////////////
};

//////////////////////////////////////////////////////////////////////////////
//
// ParticleData
//
//////////////////////////////////////////////////////////////////////////////

/* void */  ParticleData::ParticleData (void)
{
    m_pParticleBehavior = NULL;
}

//////////////////////////////////////////////////////////////////////////////
void        ParticleData::InitParticle (float fTime, ParticleBehavior* pParticleBehavior)
{
    m_fStartTime = fTime;
    m_fAngle = random (0.0f, 2.0f * pi);
    m_pParticleBehavior = pParticleBehavior;
}

//////////////////////////////////////////////////////////////////////////////
//
// ParticleGeo
//
//////////////////////////////////////////////////////////////////////////////

#define PARTICLE_BUFFER_SIZE 256

// 
// the smoke quality number is used as a multiplier for how many particles
// we will be generating in toto
#define TOTAL_PARTICLE_BUFFER_SIZE  PARTICLE_BUFFER_SIZE * 5

class ParticleGeoImpl : public ParticleGeo 
{
    private:
        // storage of the particles in a rolling buffer (eliminates oldest first when overflow occurs)
        ParticleData        m_dataParticle[TOTAL_PARTICLE_BUFFER_SIZE];
        int                 m_iIndexCurrentParticle;

        // behaviors
        SmokeBehavior       m_smokeBehavior;
        FireBehavior        m_fireBehavior;
        DamageBehavior      m_damageBehavior;
        AfterburnerBehavior m_afterburnerBehavior;
        SparkBehavior       m_sparkBehavior;

        // the current time
        float               m_fTime;

        Number* GetTime() { return Number::Cast (GetChild (0)); }

    public:
        //////////////////////////////////////////////////////////////////////
        ParticleGeoImpl(Modeler* pModeler, Number* pTime) : ParticleGeo (pTime),
            m_iIndexCurrentParticle (0),
            m_smokeBehavior (pModeler),
            m_fireBehavior (pModeler),
            m_damageBehavior (pModeler),
            m_afterburnerBehavior (pModeler),
            m_sparkBehavior (pModeler),
            m_fTime (pTime->GetValue ())
        {
        }

        //////////////////////////////////////////////////////////////////////
        ParticleData*   GetNextParticle (ParticleBehavior* pParticleBehavior)
        {
            // compute how much of the buffer we are going to use
            int iParticleBufferSize = PARTICLE_BUFFER_SIZE * ThingGeo::GetShowSmoke ();
            ZAssert (iParticleBufferSize <= TOTAL_PARTICLE_BUFFER_SIZE);
            
            // increment to the next buffer entry
            m_iIndexCurrentParticle = (m_iIndexCurrentParticle + 1) % iParticleBufferSize;

            // initialize the new particle entry
            m_dataParticle[m_iIndexCurrentParticle].InitParticle (m_fTime, pParticleBehavior);

            // return a pointer to it
            return &m_dataParticle[m_iIndexCurrentParticle];
        }

        //////////////////////////////////////////////////////////////////////
        ParticleData*   AddAfterburner (void)
        {
            return GetNextParticle (&m_afterburnerBehavior);
        }

        //////////////////////////////////////////////////////////////////////
        ParticleData*   AddDamage (void)
        {
            return GetNextParticle (&m_damageBehavior);
        }

        //////////////////////////////////////////////////////////////////////
        ParticleData*   AddSmoke (void)
        {
            return GetNextParticle (&m_smokeBehavior);
        }

        //////////////////////////////////////////////////////////////////////
        ParticleData*   AddFire (void)
        {
            return GetNextParticle (&m_fireBehavior);
        }

        //////////////////////////////////////////////////////////////////////
        ParticleData*   AddSpark (void)
        {
            return GetNextParticle (&m_sparkBehavior);
        }

        //////////////////////////////////////////////////////////////////////
        void    Evaluate (void)
        {
            m_fTime = GetTime ()->GetValue ();
        }

        //////////////////////////////////////////////////////////////////////
        ZString GetFunctionName (void) 
        { 
            return "ParticleGeo"; 
        }

        //////////////////////////////////////////////////////////////////////
        void    Render(Context* pContext)
        {
            // compute how much of the buffer we are going to use
            int iParticleBufferSize = PARTICLE_BUFFER_SIZE * ThingGeo::GetShowSmoke ();

            // loop over all of the particles
            for(int index = 0; index < iParticleBufferSize; ++index)
            {
                // some particles have shorter lives than others, so we check for an easy out
                if (m_dataParticle[index].m_pParticleBehavior)
                {
                    // compute the elapsed time of this particle
                    float   fElapsedTime = m_fTime - m_dataParticle[index].m_fStartTime;

                    // check the elapsed time against the duration of the particle
                    if (fElapsedTime <= m_dataParticle[index].m_fDuration)
                    {
                        // the particle is still alive, so render it
                        m_dataParticle[index].m_pParticleBehavior->Render (pContext, &m_dataParticle[index], fElapsedTime);
                    }
                    else
                    {
                        // the particle is expired, so clear the behavior pointer
                        // so we don't do extra work next time around
                        m_dataParticle[index].m_pParticleBehavior = NULL;
                    }
                }
            }
        }

        //////////////////////////////////////////////////////////////////////
};

//////////////////////////////////////////////////////////////////////////////
TRef<ParticleGeo> CreateParticleGeo (Modeler* pModeler, Number* pTime)
{
    return new ParticleGeoImpl (pModeler, pTime);
}

//////////////////////////////////////////////////////////////////////////////
