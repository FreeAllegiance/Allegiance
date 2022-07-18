/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	modelIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CmodelIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// modelIGC.h : Declaration of the CmodelIGC

#ifndef __MODELIGC_H_
#define __MODELIGC_H_
#include "igc.h"

template<class T> class TmodelIGC;

class   CmodelIGC
{
    public:
        CmodelIGC(ImodelIGC*    model);
        ~CmodelIGC(void);

        Time                GetMyLastUpdate(void) const
        {
            return m_lastUpdate;
        }

        void                SetMyLastUpdate(Time  lastUpdate)
        {
            m_lastUpdate = lastUpdate;
        }

        ImissionIGC*        GetMyMission(void) const
        {
            return m_pMission;
        }

        void                SetMyMission(ImissionIGC* pMission)
        {
            ZAssert (m_side == NULL);
            ZAssert (m_cluster == NULL);

            m_pMission = pMission;
        }

        void                MakeStatic(void)
        {
            ZAssert ((m_attributes & c_mtStatic) == 0);
            ZAssert ((m_attributes & c_mtHitable) == 0);
            ZAssert ((m_attributes & c_mtCastRay) != 0);

            m_pHitTest->SetID(c_htidStaticObject);
            m_pHitTest->SetStaticF(true);

            ZAssert (m_cluster);

            m_attributes &= ~c_mtCastRay;
            m_attributes |= c_mtStatic | c_mtHitable;

            m_cluster->MakeModelStatic(m_model);
        }
        void                MakeUnhitable(void)
        {
            ZAssert ((m_attributes & c_mtStatic) == 0);
            ZAssert ((m_attributes & c_mtHitable) != 0);
            ZAssert ((m_attributes & c_mtCastRay) != 0);

            ZAssert (m_cluster);

            m_attributes &= ~(c_mtCastRay | c_mtHitable);

            m_cluster->MakeModelUnhitable(m_model);
        }

    private:
        void                SetMyAttributes(ModelAttributes newVal);

    public:

    // IbaseIGC
        HRESULT         Initialize(ImissionIGC* pMission, Time now, const void*  data, int dataSize)
        {
			m_lastUpdate = now;
            m_pMission = pMission;

            return S_OK;
        }

        void            Terminate(void);

        void            Update(Time now);

    // ImodelIGC
        virtual void        LoadCVH(const char*     pszFileName,
                                    const char*     pszIconName,
                                    ModelAttributes attributes,
                                    HitTestShape    htsDefault = c_htsSphere);
        HRESULT         LoadMine(const char* textureName, float strength, float radius, const char* iconName, ModelAttributes attributes);
        HRESULT         LoadWarp(const char* textureName, const char* iconName, ModelAttributes attributes);
        HRESULT         LoadEffect(const Color& color, ModelAttributes attributes);

        HRESULT         Load(int                options,
                             const char*        model,
                             const char*        texture,
                             const char*        icon,
                             ModelAttributes    attributes);

        bool            GetVisibleF(void) const
        {
            return m_visibleF;
        }
        void            SetVisibleF(bool visibleF);
        void            SetRender(unsigned char render);

        bool            SeenBySide(IsideIGC*   side) const
        {
            if (!m_pThingSite)
                return false;
            return m_pThingSite->GetSideVisibility(side);
        }
		
		//Xynth #100 7/2010
		bool            GetCurrentEye(IsideIGC*   side) const
        {
            if (!m_pThingSite)
                return false;
            return m_pThingSite->GetCurrentEye(side);
        }

        void            UpdateSeenBySide(void)
        {
            m_pThingSite->UpdateSideVisibility(m_model, m_cluster);
        }
        void            SetSideVisibility(IsideIGC* side,
                                          bool      fVisible)
        {
            m_pThingSite->SetSideVisibility(side, fVisible);
        }

        ThingSite*      GetThingSite(void) const
        {
            return m_pThingSite;
        }
        void            FreeThingSite(void)
        {
            if (m_pHitTest)
            {
                if (m_cluster)
                {
                    m_cluster->FreeThingSite(m_attributes, m_model, m_pHitTest);
                }

                m_pHitTest->Release();
                m_pHitTest = NULL;

                ZAssert (m_pThingSite);
                m_pThingSite->Terminate();
                m_pThingSite = NULL;
            }
            else
                ZAssert (!m_pThingSite);
        }

        const Vector&   GetPosition(void) const
        {
            return m_pHitTest->GetPosition();
        }
        void            SetPosition(const Vector& newVal);

        const Vector&   GetVelocity(void) const
        {
            return m_pHitTest->GetVelocity();
        }
        void            SetVelocity(const Vector& newVal)
        {
			// mmf replaced ZAssert with log msg
		    if (!(newVal * newVal >= 0.0f)) {
				debugf("mmf Igc modelIGC.h SetVelocity: newVal^2 debug build would have called ZAssert and exited, commented out for now\n");
			}
            // ZAssert (newVal * newVal >= 0.0f);
            m_pHitTest->SetVelocity(newVal);
        }

        const Orientation&  GetOrientation(void) const
        {
            return m_pHitTest->GetOrientation();
        }
        void                SetOrientation(const Orientation& newVal);

        const Rotation&     GetRotation(void) const
        {
            return m_rotation;
        }
        void                SetRotation(const Rotation& newVal)
        {
            m_rotation = newVal;
        }
        float               GetRadius(void) const
        {
            return m_pHitTest->GetRadius();
        }
        void                SetRadius(float newVal)
        {
            ZAssert (newVal != 0.0f);
            m_pHitTest->SetRadius(newVal);
            m_pThingSite->SetRadius(newVal);
        }

        ModelAttributes     GetAttributes(void) const
        {
            return m_attributes;
        }

        IsideIGC*           GetSide(void) const
        {
            return m_side;
        }
        void                SetSide(IsideIGC* newVal)
        {
            IsideIGC*   psideOld = m_side;

            if (m_side)
                m_side->Release();

            m_side = newVal;

            if (m_side)
                m_side->AddRef();

            if (m_cluster)
            {
                //Warning ... this will probably cause a few assertion failures
                //till we get the sequence exactly right but we should never change
                //the side of an object that is in a cluster unless we are changing
                //from a legal side to a legal side.
                //ZAssert (newVal);  //Can't do this ... killing a station first sets its side to NULL
                ZAssert (psideOld);

                //Reset the thing site's view of which cluster the model is in
                //(the cluster did not change but some of the internal state for
                //the thing site has to change to reflect the change in side)
                //This also updates the side visibility for the object.
                m_pThingSite->SetCluster(m_model, m_cluster);
            }
        }

        float               GetMass(void) const
        {
            return m_mass;
        }
        void                SetMass(float    newVal)
        {
            m_mass = newVal;
        }

        IclusterIGC*        GetCluster(void) const
        {
            return m_cluster;
        }
        void                SetCluster(IclusterIGC* newVal);

        const char*         GetName(void) const
        {
            return m_name;
        }
        void                SetName(const char* newVal)
        {
            UTL::putName(m_name, newVal);
        }
        void                SetSecondaryName(const char* newVal)
        {
            ZAssert (strlen(newVal) < c_cbName-1);
            m_name[0] = '\0';

            strcpy(m_name+1, newVal);
        }

        void                SetSignature(float    newVal)
        {
            m_signature = newVal;
        }
        float               GetSignature(void) const
        {
            return m_signature;
        }
        void                ChangeSignature(float delta)
        {
            m_signature += delta;
        }

        void                SetTexture(const char* pszTextureName)
        {
            m_pThingSite->SetTexture(pszTextureName);
        }

        HRESULT                 LoadDecal(const char*      textureName,
                                          const char*      iconName,
                                          const Color&     color,
                                          bool             bDirectional,
                                          float            width,
                                          ModelAttributes  attributes,
                                          HitTestShape     htsDefault = c_htsPoint);

        void                    Move(float t)
        {
            m_pHitTest->Move(t);
            m_pThingSite->SetPosition(m_pHitTest->GetPosition());
        }
        void                    Move(void)
        {
            m_pHitTest->Move();
            m_pThingSite->SetPosition(m_pHitTest->GetPosition());
        }
        void                    SetBB(Time tStart, Time tStop, float dT)
        {
            m_pHitTest->SetBB(tStart, tStop, dT);
        }
        HitTest*                GetHitTest(void) const
        {
            return m_pHitTest;
        }

        virtual IObject*            GetIcon(void) const
        {
            return m_poIcon;
        }
        virtual void                SetIcon(IObject* picon)
        {
            m_poIcon = picon;
        }

        virtual const DamageBucketList*  GetDamageBuckets(void) const
        {
            return &m_damageBuckets;
        }
        virtual void                     AddDamageBucket(DamageBucket*   pdb)
        {
            ZAssert (m_pMission);
            m_damageBuckets.last(pdb);
        }
        virtual void                     DeleteDamageBucket(DamageBucket*   pdb)
        {
            DamageBucketLink*   pdbl = m_damageBuckets.find(pdb);
            ZAssert (pdbl);
            delete pdbl;
        }

    private:
        ImissionIGC*        m_pMission;
        Time                m_lastUpdate;

        TRef<ThingSite>     m_pThingSite;
        TRef<IObject>       m_poIcon;
        IsideIGC*           m_side;

        DamageBucketList    m_damageBuckets;

        ImodelIGC*          m_model;        //pointer for anything that needs a pointer to the "model" & not the class implementation
        Rotation            m_rotation;
        IclusterIGC*        m_cluster;
        float               m_mass;
        float               m_signature;
        HitTest*            m_pHitTest;
        char                m_name[c_cbName];
        ModelAttributes     m_attributes;
        bool                m_visibleF;
        bool                m_selectedF;
        bool                m_decalF;
        unsigned char       m_render;
};

template<class T> class TmodelIGC : public T
{
    public:
        TmodelIGC(void)
        :
            m_model(this)
        {
        }

        //Note ... these methods are not part of the ImodelIGC interface (& not virtual either)
        Time                GetMyLastUpdate(void) const
        {
            return m_model.GetMyLastUpdate();
        }

        void                SetMyLastUpdate(Time  lastUpdate)
        {
            m_model.SetMyLastUpdate(lastUpdate);
        }

        ImissionIGC*        GetMyMission(void) const
        {
            return m_model.GetMyMission();
        }

        void                SetMyMission(ImissionIGC* pMission)
        {
            m_model.SetMyMission(pMission);
        }

        void                MakeStatic(void)
        {
            m_model.MakeStatic();
        }

        void                MakeUnhitable(void)
        {
            m_model.MakeUnhitable();
        }

    // IbaseIGC
        virtual HRESULT Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
        {
            return m_model.Initialize(pMission, now, data, dataSize);
        }
        virtual void    Terminate(void)
        {
            m_model.Terminate();
        }
        virtual void    Update(Time now)
        {
            m_model.Update(now);
        }

        virtual ImissionIGC*        GetMission(void) const
        {
            return m_model.GetMyMission();
        }

    // ImodelIGC
        virtual Time                 GetLastUpdate(void) const
        {
            return m_model.GetMyLastUpdate();
        }
        virtual void                 SetLastUpdate(Time now)
        {
            m_model.SetMyLastUpdate(now);
        }

        virtual void        SetMission(ImissionIGC* pMission)
        {
            m_model.SetMyMission(pMission);
        }
        virtual void        LoadCVH(const char*     pszFileName,
                                    const char*     pszIconName,
                                    ModelAttributes attributes,
                                    HitTestShape    htsDefault = c_htsSphere)
        {
            m_model.LoadCVH(pszFileName, pszIconName, attributes, htsDefault);
        }
        virtual HRESULT              LoadEffect(const Color& color, ModelAttributes attributes)
        {
            return m_model.LoadEffect(color, attributes);
        }
        virtual HRESULT              LoadMine(const char* textureName, float strength, float radius, const char* icon, ModelAttributes attributes)
        {
            return m_model.LoadMine(textureName, strength, radius, icon, attributes);
        }
        virtual HRESULT              LoadWarp(const char* textureName, const char* icon, ModelAttributes attributes)
        {
            return m_model.LoadWarp(textureName, icon, attributes);
        }

        virtual HRESULT              Load(int                  options,
                                          const char*          model,
                                          const char*          texture,
                                          const char*          icon,
                                          ModelAttributes      attributes)
        {
            return m_model.Load(options, model, texture, icon, attributes);
        }

        virtual bool                 GetVisibleF(void) const
        {			
            return m_model.GetVisibleF();
        }
		//Xynth #100 7/2010
		virtual bool                 GetCurrentEye(IsideIGC*   side) const
        {
			return m_model.GetCurrentEye(side);
		}
        virtual void                 SetVisibleF(bool visibleF)
        {
            m_model.SetVisibleF(visibleF);
        }
        virtual void                 SetRender(unsigned char render)
        {
            m_model.SetRender(render);
        }

        virtual bool                 SeenBySide(IsideIGC*  side) const
        {
            return m_model.SeenBySide(side);
        }
        virtual void                 UpdateSeenBySide(void)
        {
            m_model.UpdateSeenBySide();
        }
        virtual void                 SetSideVisibility(IsideIGC* side, bool fVisible)
        {
            m_model.SetSideVisibility(side, fVisible);
        }

        virtual ThingSite*            GetThingSite(void) const
        {
            return m_model.GetThingSite();
        }

        virtual void                  FreeThingSite(void)
        {
            m_model.FreeThingSite();
        }

        virtual const Vector&        GetPosition(void) const
        {
            return m_model.GetPosition();
        }
        virtual void                 SetPosition(const Vector& newVal)
        {
            m_model.SetPosition(newVal);
        }

        virtual const Vector&        GetVelocity(void) const
        {
            return m_model.GetVelocity();
        }
        virtual void                 SetVelocity(const Vector& newVal)
        {
            m_model.SetVelocity(newVal);
        }

        virtual const Orientation&   GetOrientation(void) const
        {
            return m_model.GetOrientation();
        }
        virtual void                 SetOrientation(const Orientation& newVal)
        {
            m_model.SetOrientation(newVal);
        }

        virtual const Rotation&      GetRotation(void) const
        {
            return m_model.GetRotation();
        }
        virtual void                 SetRotation(const Rotation& newVal)
        {
            m_model.SetRotation(newVal);
        }
        virtual float                GetRadius(void) const
        {
            return m_model.GetRadius();
        }
        virtual void                 SetRadius(float newVal)
        {
            m_model.SetRadius(newVal);
        }

        virtual ModelAttributes      GetAttributes(void) const
        {
            return m_model.GetAttributes();
        }

        virtual IsideIGC*            GetSide(void) const
        {
            return m_model.GetSide();
        }
        virtual void                 SetSide(IsideIGC* newVal)
        {
            m_model.SetSide(newVal);
        }

        virtual float                GetMass(void) const
        {
            return m_model.GetMass();
        }
        virtual void                 SetMass(float newVal)
        {
            m_model.SetMass(newVal);
        }

        virtual IclusterIGC*         GetCluster(void) const
        {
            return m_model.GetCluster();
        }
        virtual void                 SetCluster(IclusterIGC* newVal)
        {
            m_model.SetCluster(newVal);
        }

        virtual char const*          GetName(void) const
        {
            return m_model.GetName();
        }
        virtual void                 SetName(const char* newVal)
        {
            m_model.SetName(newVal);
        }
        void                SetSecondaryName(const char* newVal)
        {
            m_model.SetSecondaryName(newVal);
        }
        virtual float                GetSignature(void) const
        {
            return m_model.GetSignature();
        }
        virtual void                 SetSignature(float newVal)
        {
            m_model.SetSignature(newVal);
        }

        virtual void                 ChangeSignature(float delta)
        {
            m_model.ChangeSignature(delta);
        }

        virtual void                SetTexture(const char* pszTextureName)
        {
            m_model.SetTexture(pszTextureName);
        }

        virtual HRESULT              LoadDecal(const char*      textureName,
                                               const char*      iconName,
                                               const Color&     color,
                                               bool             bDirectional,
                                               float            width,
                                               ModelAttributes  attributes,
                                               HitTestShape     htsDefault = c_htsPoint)
        {
            return m_model.LoadDecal(textureName, iconName, color, bDirectional, width, attributes, htsDefault);
        }

        virtual void                 Move(float t)
        {
            m_model.Move(t);
        }
        virtual void                 Move(void)
        {
            m_model.Move();
        }
        virtual void                 SetBB(Time tStart, Time tStop, float dt)
        {
            m_model.SetBB(tStart, tStop, dt);
        }
        virtual HitTest*             GetHitTest(void) const
        {
            return m_model.GetHitTest();
        }

        virtual void                 HandleCollision(Time       timeCollision,
                                                     float                  tCollision,
                                                     const CollisionEntry&  entry,
                                                     ImodelIGC* pModel)
        {
            //Should never be called
            ZAssert (false);
        }

        virtual IObject*            GetIcon(void) const
        {
            return m_model.GetIcon();
        }

        virtual void                    SetIcon(IObject* picon)
        {
            m_model.SetIcon(picon);
        }

        virtual const DamageBucketList*  GetDamageBuckets(void) const
        {
            return m_model.GetDamageBuckets();
        }
        virtual void                     AddDamageBucket(DamageBucket*   pdb)
        {
            m_model.AddDamageBucket(pdb);
        }
        virtual void                     DeleteDamageBucket(DamageBucket*   pdb)
        {
            m_model.DeleteDamageBucket(pdb);
        }

    private:
        CmodelIGC   m_model;
};

#endif //__MODELIGC_H_
