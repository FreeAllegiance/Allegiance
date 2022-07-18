/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	asteroidIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CasteroidIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// asteroidIGC.h : Declaration of the CasteroidIGC

#ifndef __ASTEROIDIGC_H_
#define __ASTEROIDIGC_H_

#include "igc.h"
#include "modelIGC.h"

class   CasteroidIGC : public TmodelIGC<IasteroidIGC>
{
    public:
        CasteroidIGC(void)
        :
            m_fraction(1.0f)
		{
        }

    // IbaseIGC
	    virtual HRESULT Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize);
        virtual void    Terminate(void)
        {
            if (m_pbuildingEffect)
            {
                //The building effect gets terminated along with the asteroid
                m_pbuildingEffect->Terminate();
                ZAssert (m_pbuildingEffect == NULL);
            }

            GetMyMission()->DeleteAsteroid(this);
            TmodelIGC<IasteroidIGC>::Terminate();
        }
        virtual int     Export(void*    data) const;

        virtual void    Update(Time now)
        {
            if (((m_asteroidDef.aabmCapabilities & c_aabmMineHe3) != 0) &&
                (m_asteroidDef.ore < m_asteroidDef.oreMax))
            {
                float   dOre = (now - GetMyLastUpdate()) * GetMyMission()->GetFloatConstant(c_fcidHe3Regeneration);

                dOre *= (m_asteroidDef.oreMax - m_asteroidDef.ore);
                m_asteroidDef.ore += dOre;
            }
			
			//Xynth #225 9/10 Don't update He3 for the first 3.0 seconds you enter a sector
			if (m_inhibitUpdate)
			{
				if (m_inhibitCounter == -1)
					m_inhibitCounter = Time::Now();
				else if (fabs(m_inhibitCounter - Time::Now()) > 3.0)
				{
					m_inhibitCounter = -1;
					m_inhibitUpdate = false;
				}
			}

			//Xynth #100 7/2010 loop through sides to update the ore they know about
			if ((m_asteroidDef.aabmCapabilities & c_aabmMineHe3) != 0)
			{
				for (SideLinkIGC* psl = this->GetMission()->GetSides()->first(); psl != NULL; psl = psl->next())
				{
					IsideIGC* pside = psl->data();
					if (this->GetCurrentEye(pside) && !m_inhibitUpdate)
					{						
						oreSeenBySide.Set(pside, m_asteroidDef.ore);						
					}

				}
			}


            TmodelIGC<IasteroidIGC>::Update(now);

            //If there is an active building effect ... call the building effect to do the 
            //update so that ... if the building effect completes ... it is being done from
            //the asteroid update
            if (m_pbuildingEffect)
                m_pbuildingEffect->AsteroidUpdate(now);
        }

        virtual ObjectType  GetObjectType(void) const
        {
            return OT_asteroid;
        }

        virtual ObjectID    GetObjectID(void) const
        {
            return m_asteroidDef.asteroidID;
        }

    // ImodelIGC
        virtual void    SetCluster(IclusterIGC* cluster)
        {
            AddRef();

            //Overrride the model's cluster method so that we can maintain the cluster's asteroid list
            //(as well as letting the model maintain its model list)
            {
                IclusterIGC*    c = GetCluster();
                if (c)
                    c->DeleteAsteroid(this);
            }

            TmodelIGC<IasteroidIGC>::SetCluster(cluster);

            if (cluster)
                cluster->AddAsteroid(this);

            Release();
        }

    // IdamageIGC
        virtual DamageResult        ReceiveDamage(DamageTypeID            type,
                                                  float                   amount,
                                                  Time                    timeCollision,
                                                  const Vector&           position1,
                                                  const Vector&           position2,
                                                  ImodelIGC*              launcher)
        {
            DamageResult    dr = c_drHullDamage;

            amount *= GetMyMission()->GetDamageConstant(type, c_defidAsteroid);

            if (amount < 0.0f)
            {
                m_fraction -= amount / float(m_asteroidDef.hitpoints);
                if (m_fraction > 1.0f)
                    m_fraction = 1.0f;
            }
            else if (amount != 0.0f)
            {
                float   oldFraction = m_fraction;

                ZAssert (m_asteroidDef.hitpoints != 0);
                m_fraction -= amount / float(m_asteroidDef.hitpoints);

                GetMission()->GetIgcSite()->PlaySoundEffect(rockHitSound, this,
                    GetOrientation().TimesInverse(position2 - position1));

                if (m_fraction <= 0.0f)
                {
                    m_fraction = 0.0f;
                    if (oldFraction > 0.0f)
                    {
                        GetMyMission()->GetIgcSite()->KillAsteroidEvent(this, true);
                        dr = c_drKilled;
                    }
                }
                
                return ((type & c_dmgidNoDebris) == 0);
            }

            return dr;
        }
        virtual float   GetFraction(void) const
        {
            return m_fraction;
        }
        virtual void    SetFraction(float newVal)
        {
            m_fraction = newVal;
        }

        virtual float               GetHitPoints(void) const
        {
            return m_fraction * float(m_asteroidDef.hitpoints);
        }

    // IasteroidIGC
        virtual float   GetOre(void) const
        {
            return m_asteroidDef.ore;
        }
        virtual void    SetOre(float    newVal)
        {
            /* NYI -- re-enable if He3 does not regenerate
            if ((newVal == 0.0f) &&
                ((m_asteroidDef.aabmCapabilities & (c_aabmMineHe3 | c_aabmMineGold)) != 0))
            {
                m_asteroidDef.aabmCapabilities &= ~(c_aabmMineHe3 | c_aabmMineGold);
                char    bfr[c_cbName];
                strcpy(bfr, GetName());
                SetSecondaryName(bfr);

                const AsteroidDef&  ad = IasteroidIGC::GetTypeDefaults(m_asteroidDef.aabmCapabilities);

                SetIcon(GetMyMission()->GetIgcSite()->LoadRadarIcon(ad.iconName));
            }
            */
			
            m_asteroidDef.ore = newVal;
			//Xynth #100 7/2010 Loop through sides to update ore seen by any sides eyeing asteroid
			//Xynth #225 handle updates in Update function
			/*for (SideLinkIGC* psl = this->GetMission()->GetSides()->first(); psl != NULL; psl = psl->next())
			{
				IsideIGC* pside = psl->data();
				if (this->GetCurrentEye(pside))
				{						
					oreSeenBySide.Set(pside, m_asteroidDef.ore);						
				}

			}*/
			}
        virtual float   MineOre(float    newVal)
        {
            if (m_asteroidDef.ore < newVal)
            {
                newVal = m_asteroidDef.ore;
                m_asteroidDef.ore = 0.0f;

                /* NYI -- re-enable if He3 does not regenerate
                GetMyMission()->GetIgcSite()->DrainAsteroidEvent(this);
                */
            }
            else
                m_asteroidDef.ore -= newVal;
			
			//Xynth #100 7/2010 Loop through sides to update ore seen by any sides eyeing asteroid
			//Xynth #225 Handle updates in Update function
			/*for (SideLinkIGC* psl = this->GetMission()->GetSides()->first(); psl != NULL; psl = psl->next())
			{
				IsideIGC* pside = psl->data();
				if (this->GetCurrentEye(pside))
				{						
					oreSeenBySide.Set(pside, m_asteroidDef.ore);						
				}

			}*/
			//Xynth #132 7/2010  Update asteroid periodically
			if (fabs(m_asteroidDef.ore - m_lastUpdateOre) > 3.0)
			{
				GetMyMission()->GetIgcSite()->MineAsteroidEvent(this, this->GetOreFraction());
				m_lastUpdateOre = m_asteroidDef.ore;
			}
            return newVal;
        }

        virtual AsteroidAbilityBitMask  GetCapabilities(void) const
        {
            return m_pbuildingEffect ? 0 : m_asteroidDef.aabmCapabilities;
        }
        virtual bool                    HasCapability(AsteroidAbilityBitMask aabm) const
        {
            return m_pbuildingEffect ? false : ((m_asteroidDef.aabmCapabilities & aabm) != 0);
        }

        virtual IbuildingEffectIGC*     GetBuildingEffect(void) const
        {
            return m_pbuildingEffect;
        }
        virtual void                    SetBuildingEffect(IbuildingEffectIGC* pbe)
        {
            if (m_pbuildingEffect)
            {
                ZAssert (pbe == NULL);
                m_pbuildingEffect->MakeUnhitable();
            }

            m_pbuildingEffect = pbe;
        }

		//Xynth #100 7/2010
		virtual float GetOreSeenBySide(IsideIGC *side1) const
		{			
			float oreSeen;
			oreSeenBySide.Find(side1, oreSeen);
			return oreSeen;
		}

		virtual bool GetAsteroidCurrentEye(IsideIGC *side1) const
		{
			return this->GetCurrentEye(side1);
		}

		//Xynth #163 7/2010
		virtual void SetOreWithFraction(float oreFraction, bool clientUpdate)
		{
			m_asteroidDef.ore = oreFraction * m_asteroidDef.oreMax;
			//Loop through sides to update ore seen by any sides eyeing asteroid
			for (SideLinkIGC* psl = this->GetMission()->GetSides()->first(); psl != NULL; psl = psl->next())
			{
				IsideIGC* pside = psl->data();
				if (this->GetCurrentEye(pside) || clientUpdate) //Xynth #225 make sure client updates rock
				{						
					oreSeenBySide.Set(pside, m_asteroidDef.ore);						
				}
			}			
			}

		//Xynth #163 7/2010
		virtual float GetOreFraction() const
		{
			return m_asteroidDef.ore / m_asteroidDef.oreMax;
		}

		//Imago 8/10
		void SetBuilderSeenSide(ObjectID oid) { m_builderseensides[oid] = true; }
		bool GetBuilderSeenSide(ObjectID oid) { return m_builderseensides[oid]; }
		//
		//Xynth #225 9/10
		virtual void SetInhibitUpdate(bool inhib) {m_inhibitUpdate = inhib;}

    private:
        AsteroidDef                 m_asteroidDef;
		//Xynth #100 7/2010 array to hold what each team knows about ore in this rock		
		TMap<IsideIGC*, float> oreSeenBySide;
		float                       m_lastUpdateOre;  //Xynth #132 7/2010 ore last time update was sent out
        float                       m_fraction;
        TRef<IbuildingEffectIGC>    m_pbuildingEffect;
		bool						m_builderseensides[c_cSidesMax]; //Imago #120 #121
		bool                        m_inhibitUpdate; //Xynth #225 bookkeeping variables to prevent illegal or update		
		Time					    m_inhibitCounter; //upon entering cluster		
};

#endif //__ASTEROIDIGC_H_
