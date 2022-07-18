/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	missileIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CmissileIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// missileIGC.cpp : Implementation of CmissileIGC
#include "missileigc.h"

/////////////////////////////////////////////////////////////////////////////
// CmissileIGC
CmissileIGC::CmissileIGC(void)
:
    m_launcher(NULL),
    m_target(NULL),
    m_missileType(NULL)
{
}

CmissileIGC::~CmissileIGC(void)
{
}

HRESULT CmissileIGC::Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
{
    TmodelIGC<ImissileIGC>::Initialize(pMission, now, data, dataSize);

    ZRetailAssert (data && (dataSize == sizeof(DataMissileIGC)));
    {
        DataMissileIGC*  dataMissile = (DataMissileIGC*)data;

        m_missileType = dataMissile->pmissiletype;
        ZAssert (m_missileType);
        ZAssert (m_missileType->GetObjectType() == OT_missileType);
        m_missileType->AddRef();

        DataMissileTypeIGC*  dataMissileType = (DataMissileTypeIGC*)(m_missileType->GetData());
        ZAssert (dataMissileType);

        //Load the model for the missile
        HRESULT hr;
        if (iswalpha(dataMissileType->modelName[0])) {
            hr = Load(0, dataMissileType->modelName, dataMissileType->textureName, dataMissileType->iconName,
                      c_mtDamagable | c_mtHitable | c_mtCastRay | c_mtSeenBySide);

            GetThingSite()->SetAfterburnerSmokeSize ( 0.25f + (GetRadius () * 0.1f));
            GetThingSite()->SetAfterburnerFireDuration (0.33f);
            GetThingSite()->SetAfterburnerSmokeDuration (2.0f);
        }
        else
        {
            //  , should pass in blend color from database
            hr = LoadDecal(dataMissileType->textureName, dataMissileType->iconName, 
                           Color((float)dataMissileType->color.r,
                                 (float)dataMissileType->color.g,
                                 (float)dataMissileType->color.b,
                                 (float)dataMissileType->color.a),
                           dataMissileType->bDirectional,
                           dataMissileType->width,
                           c_mtDamagable | c_mtHitable | c_mtCastRay | c_mtSeenBySide,
                           c_htsSphere);
        }

        ZAssert (SUCCEEDED(hr));
        SetRadius(dataMissileType->radius);
        SetSignature(dataMissileType->signature);
        m_fraction = 1.0f;

        SetPosition(dataMissile->position);
        SetVelocity(dataMissile->velocity);
        {
            Orientation o(dataMissile->forward);
            SetOrientation(o);
        }
        {
            Rotation    r(dataMissile->forward, dataMissileType->rotation);
            SetRotation(r);
        }

        m_launcher = dataMissile->pLauncher;
        ZAssert (m_launcher);
        m_launcher->AddRef();
        {
            HitTest*    ht = GetHitTest();

            //lifespan == 0 => immortal missile that can hit until it gets terminated on the next update; this is bad
            ZAssert (dataMissileType->lifespan > 0.0f);
            m_timeExpire = now + dataMissileType->lifespan;
            ZAssert (m_timeExpire != now);

            //Can't hit your launcher
            ht->SetNoHit(m_launcher->GetHitTest());
            SetSide(m_launcher->GetSide());
        }
        m_launcher->SetLastMissileFired(this);

        m_timeActivate = now + dataMissileType->readyTime;

        SetCluster(dataMissile->pCluster);

        if (dataMissile->pTarget && (dataMissile->pTarget->GetCluster() == dataMissile->pCluster))
        {
            m_target = dataMissile->pTarget;
            m_target->AddRef();

            //Estimate the time of impact with the target
			// mmf / Tkela modifications for zero accel
            m_tImpact = dataMissileType->readyTime +
						((dataMissileType->acceleration <= 0.0f)
						? 0.0f 
						: (float)sqrt((2.0f / dataMissileType->acceleration) *
						(m_target->GetPosition() -
                        dataMissile->position +
                        dataMissileType->readyTime * (m_target->GetVelocity() - dataMissile->velocity)).Length()));
        }

        m_lock = dataMissile->lock;

        m_missileID = dataMissile->missileID;

        m_bDisarmed = dataMissile->bDud;

        SetMass(0.0f);
    }

    return S_OK;
}

void    CmissileIGC::Terminate(void)
{
    AddRef();

	TmodelIGC<ImissileIGC>::Terminate();

    if (m_launcher)
    {
        if (m_launcher->GetLastMissileFired() == this)
            m_launcher->SetLastMissileFired(NULL);

        m_launcher->Release();
        m_launcher = NULL;
    }

    if (m_target)
    {
        m_target->Release();
        m_target = NULL;
    }

    if (m_missileType)
    {
        m_missileType->Release();
        m_missileType = NULL;
    }

    Release();
}

double   NewtonA(double a, double b, double c, double d, double tOld)
{
    //Solve:
    //  f(t)  = a + bt + c t^2 - d t^4 = 0
    //  f'(t) = b + 2c t - 4d t^3
    double   t0;
    double   t1 = tOld;

    //NYI not that we should need it ...
    int nCycles = 0;
    do
    {
        t0 = t1;

        double   p2 = t0 * t0;   //t0^2
        double   p3 = p2 * t0;   //t0^3
        double   p4 = p3 * t0;   //t0^4

        double  f = a + b * t0 + c * p2 - d * p4;
        if (fabs(f) < 0.1)
            break;              //close enough

        double  dfdt = b + 2.0 * c * t0 - 4.0 * d * p3;
        double  delta = f / dfdt;

        t1 = t0 - delta;
    }
    while (++nCycles < 20);

    if (nCycles >= 20)
    {
		// mmf commented this out, occurs way too often in 'normal' play, especially with quickfires
		// we can stick it back in if someone wants to work on this code and needs this info
        // debugf("**** Timeout NewtonA(%f %f %f %f %f) %f %f\n", a, b, c, d, tOld, t0, t1);
    }

    return t0;
}

double   NewtonB(double a, double b, double c, double d, double tOld, double ti)
{
    //Solve:
    //  f(t)  = a + bt + c t^2 - d t^4 = 0
    //  f'(t) = b + 2c t - 4d t^3
    //
    // but, as a special case, quit if dfdt ever >= 0.0
    double   t0;
    double   t1 = tOld;

    //NYI not that we should need it ...
    int nCycles = 0;
    do
    {
        t0 = t1;

        double   p2 = t0 * t0;   //t0^2
        double   p3 = p2 * t0;   //t0^3
        double   p4 = p3 * t0;   //t0^4

        double  f = a + b * t0 + c * p2 - d * p4;
        if (fabs(f) < 0.1)
            break;              //close enough

        double  dfdt = b + 2.0 * c * t0 - 4.0 * d * p3;
        if (dfdt >= 0.0)
        {
            //debugf("**** Aborted NewtonB(%f %f %f %f %f) %f %f %f\n", a, b, c, d, dfdt, tOld, t0, t1);
            return -1.0;
        }
        double  delta = f / dfdt;

        t1 = t0 - delta;

        //This is not strictly required (if t is past the inflection point and the slope is negative,
        //then there isn't any root before the inflection point to find) ... but it is better to start
        //with a more accurate initial guess for t when searching for a root past the inflection point.
        if (t1 > ti)
        {
            //debugf("**** Aborted NewtonB(%f %f %f %f %f) %f %f %f\n", a, b, c, d, dfdt, tOld, t0, t1);
            return -1.0;
        }
    }
    while (++nCycles < 20);

    if (nCycles >= 20)
    {
		// mmf commented this out, occurs too often in 'normal' play, especially with quickfires
		// we can stick it back in if someone wants to work on this code and needs this info
        // debugf("**** Timeout NewtonA(%f %f %f %f %f) %f %f\n", a, b, c, d, tOld, t0, t1);
        debugf("**** Timeout NewtonB(%f %f %f %f %f) %f %f\n", a, b, c, d, tOld, t0, t1);
    }

    return t0;
}
static float    solve(const Vector& dP,
                      const Vector& dV,
                      float         acceleration,
                      float         tOld)
{
    ZAssert (acceleration > 0.0f);

    //General problem when will the missile hit the target
    //  Solve |dP + t * dV| = 1/2 acceleration t^2
    //    F1(t) = dP * dP + 2 * t * dP * dV + t^2 * dV * dV = a + b t + c t^2
    //    F2(t) = 0.25 acceleration^2 * t^4                 = d t^4

    //Solve for t such that F1(t) == F2(t)

    double  a = dP * dP;
    double  c = dV * dV;
    double  d = acceleration * acceleration / 4.0;
    if (c == 0.0)
    {
        //Let's eliminate one special case upfront: c = 0 implies a non-moving target (or no lock)
        //Either way ... it makes solving the problem easy
        //  c = 0 => b = 0
        //  a = d t^4
        //  t = (a/d)^(1/4)

        return float(pow(a/d, 0.25));
    }

    double  b = 2.0 * (dP * dV);

    //F(t) = F1(t) - F2(t) = a + b t + c t^2 - d t^4
    //F'(t)                =     b   + 2c t  - 4d t^3
    //F''(t)               = 2c - 12 d t^2

    //First ... find the inflection points ... F''(t) = 0
    // since c, d > 0, there is exactly one > 0.0f
    double  ti = sqrt(c / (6.0 * d));
    
    //What is the slope at the inflection point?
    double  dftp = b + (4.0/3.0) * c * ti;         //Simplifying a little F'(ti) == b + 2c ti - 4d ti^3
    
    const double epsilon = -0.001;
    if (dftp <= epsilon)
    {
        //OK ... easy case: F'(t) at its local maximum for t > 0 is < 0
        //Therefore F'(t) < 0 for all t > 0 and Newton's works very nicely ... use it

        return float(NewtonA(a, b, c, d, tOld));
    } 

    if (b < 0.0)
    {
        //OK ... hard case #1 ... F'(0) = b < 0 & F'(ti) > epsilon
        //so there is a local minima in [0, ti] ... which means that there might be a solution
        //in [0, tmn] where F'(tmn) = 0

        //Unfortunately, tmn is not trivial to find, so we fake it:
        //Pick t in [0, ti] such that F'(t) < 0

        double  t = tOld;       //Try our initial guess at a solution
        if ((t < ti) && (b + 2.0 * c * t - 4.0 * d * t * t * t > epsilon))
            t = 0.0f;           //Didn't work: fall back to 0 (which is known to be good)

        //Use Newton's with a twist ... if F'(t) >= 0.0 then we can give up (because, barring round-off,
        //we'll never overshoot a F(t) = 0 since F''(t) > 0)

        t = NewtonB(a, b, c, d, t, ti);
        if (t >= 0.0f)
            return float(t);
    }

    //OK ... hard case #2 ... F'(ti) > epsilon
    // so there is a local maxima in [ti, infinity] ... which means there might be a solution
    // in [ti, tmx] and there will be one in [tmx, infinity], where F'(tmx) = 0
    //
    // Except that, since we got here, we are reasonably sure that the there is no solution in [ti, tmx]
    // (since, if so, there would also be another solution in [0, tmn] & we didn't find it above)
    //
    // So ... try to find a solution starting at t, where F'(t) < epsilon

    //Start past the inflection point
    double  t = tOld;
    if (t < ti)
        t = ti + 1.0;

    //Search for the a time with a sufficiently negative slope
    while (b + 2.0 * c * t - 4.0 * d * t * t * t > epsilon)
        t += 1.0;                            //Keep incrementing till we get it right

    //OK ... we found a good starting value.
    // Because F''(t) < 0 for all t > ti, we don't have to worry about local maxima
    return float(NewtonA(a, b, c, d, t));
}

void    CmissileIGC::Update(Time now)
{
    if (GetMyLastUpdate() >= m_timeExpire)
    {
        Explode(GetPosition());
        Terminate();
    }
    else if (now >= GetMyLastUpdate())
    {
        if (now > m_timeActivate)
        {
            //Actively track the target
            Orientation o = GetOrientation();
            float   acceleration = m_missileType->GetAcceleration();

            float dt = now - ((m_timeActivate > GetMyLastUpdate()) ? m_timeActivate : GetMyLastUpdate());

            if (m_target && (m_target->GetCluster() != GetCluster()))
            {
                m_target->Release();
                m_target = NULL;    //lost our target
            }

            const Vector&   myPosition = GetPosition();
            const Vector&   myVelocity = GetVelocity();

			// mmf / Tkela modifications for zero accel
            if (m_target && (acceleration > 0.0f))
            {
                //Where will our target be when we get there ... try several passes
                //adjusting our speed (to account for velocity).
                Vector  dP = m_target->GetPosition() - myPosition;
                Vector  dV = m_target->GetVelocity() * m_lock - myVelocity;
                m_tImpact = solve(dP, dV, acceleration, m_tImpact);

                o.TurnTo(dP + m_tImpact * dV, m_missileType->GetTurnRate() * dt * GetSide()->GetGlobalAttributeSet().GetAttribute(c_gaTurnRateMissile));

                SetOrientation(o);
            }
            const Vector&   backward = o.GetBackward();

            //Kick in the primary thrusters
            Vector  deltaV = backward * (acceleration * dt);
            SetVelocity(myVelocity - deltaV);

            if (m_timeActivate > GetMyLastUpdate())
            {
                //The missile activated partway through the update cycle. Fudge things (by a teleport)
                //so that the missile ends up at the position it would have ended up if the activate had
                //happened exactly at an update boundary
                SetPosition(myPosition - deltaV * (m_timeActivate - GetMyLastUpdate()));
            }

            m_tImpact -= dt;

            {
                Rotation    r = GetRotation();
                r.axis(o.GetBackward());
                SetRotation(r);
            }

            ZAssert (GetThingSite());
            GetThingSite()->SetAfterburnerThrust(backward, 1.0f);
        }

        TmodelIGC<ImissileIGC>::Update(now);
    }
}

void                 CmissileIGC::HandleCollision(Time       timeCollision,
                                                  float                  tCollision,
                                                  const CollisionEntry&  entry,
                                                  ImodelIGC* pModel)
{
    if (!m_bDisarmed)
    {
        ObjectType  ot = pModel->GetObjectType();
		// KGJV - experimental
		if (GetMyMission()->GetMissionParams()->bExperimental)
		{
			// ignore minefields except those with c_eabmShootMissiles capability
			if (ot == OT_mine)
			{
				ImineIGC*    pmine = (ImineIGC*)pModel;
				if(!pmine->GetMineType()->HasCapability(c_eabmShootMissiles)) 
					return;             //Ignore collisions with minefields
				if ((GetSide()==pModel->GetSide()) || IsideIGC::AlliedSides(GetSide(),pModel->GetSide())) // #ALLY -was: GetSide()==pModel->GetSide() imago fixed 7/8/09
					return;             // ignore if same side
			}
		} // experimental
		else
		// original code here
        if (ot == OT_mine)
			return;             //Ignore collisions with minefields

        IIgcSite*   pigc = GetMyMission()->GetIgcSite();
        Vector position1;

        DamageTypeID dmgid = m_missileType->GetDamageType();
        {
            //A missile hit something
            position1 = GetPosition() + GetVelocity() * tCollision;
            Vector position2 = pModel->GetPosition() + pModel->GetVelocity() * tCollision;
            if (ot != OT_missile)
            {
                ExpendableAbilityBitMask    eabm = m_missileType->GetCapabilities();
                if (eabm & c_eabmWarpBomb)
                {
                    //Hack so that the client will not show an explosion
                    position1 = Vector::GetZero();

                    if (ot == OT_warp)
                        pigc->WarpBombEvent((IwarpIGC*)pModel, this);
                }
                else if (pModel->GetAttributes() & c_mtDamagable)
                {
                    //The target can take damage
                    IdamageIGC*   pDamage = (IdamageIGC*)pModel;

                    pDamage->ReceiveDamage(dmgid,
                                           m_missileType->GetPower() * GetSide()->GetGlobalAttributeSet().GetAttribute(c_gaDamageMissiles),
                                           timeCollision,
                                           position2,
                                           position1,
                                           m_launcher);

                    if (eabm & c_eabmCapture)
                    {
                        //Hack so that the client will not show an explosion
                        position1 = Vector::GetZero();

                        if ((ot == OT_station) && m_launcher)
                        {
                            IstationIGC*    pstation = (IstationIGC*)pModel;

                            if ((!GetMyMission()->GetMissionParams()->bInvulnerableStations) &&
                                (!pstation->GetStationType()->HasCapability(c_sabmPedestal)) &&
                                (pstation->GetShieldFraction() < GetMyMission()->GetFloatConstant(c_fcidDownedShield)))
                            {
                                if ((GetSide()!=pModel->GetSide()) || IsideIGC::AlliedSides(GetSide(),pModel->GetSide()))
								  pigc->CaptureStationEvent(m_launcher, pstation); // Andon: Fix nerve gas self-capture bug 7/10
                            }
                        }
                    }
                }
            }
            else if (m_launcher->GetObjectID() == ((ImissileIGC*)pModel)->GetLauncher()->GetObjectID())
			{
				return;
			}
			else
            {
                //Missiles hitting missiles are a special case: both die without calling either's receive damage method.
                //Create explosions for both missiles
                pigc->KillMissileEvent(((ImissileIGC*)pModel), position2);
            }
        }

        pigc->KillMissileEvent(this, position1);
    }
}

void                 CmissileIGC::Explode(const Vector& position)
{
    //Boom
    float   p = m_missileType->GetBlastPower();
    if (p != 0.0f)
    {
        GetCluster()->CreateExplosion(m_missileType->GetDamageType(),
                                      p * GetSide()->GetGlobalAttributeSet().GetAttribute(c_gaDamageMissiles),
                                      m_missileType->GetBlastRadius(),
                                      c_etMissile,
                                      GetMyLastUpdate(),
                                      position,
                                      m_launcher);
    }
    else
        GetCluster()->GetClusterSite()->AddExplosion(position, GetRadius(), c_etMissile);
}
