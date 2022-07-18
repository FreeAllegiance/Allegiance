/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:    modelIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CmodelIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
#include "modelIGC.h"
#include <stdio.h>
#include <math.h>

/////////////////////////////////////////////////////////////////////////////
// CmodelIGC
CmodelIGC::CmodelIGC(ImodelIGC* model)
:
    m_model(model),
    m_visibleF(true),
    m_render(c_ucRenderAll),
    m_selectedF(false),
    m_attributes(0),
    m_side(NULL),
    m_cluster(NULL),
    m_decalF(false),
    m_pHitTest(NULL),
    m_rotation(c_rotationZero),
    m_signature(1.0f),
    m_pThingSite(NULL)
{
    m_name[0] = '\0';
}

CmodelIGC::~CmodelIGC(void)
{
    if (m_pHitTest)
    {
        ZAssert (!m_cluster);
        m_pHitTest->Release();
    }
}

void    CmodelIGC::Terminate(void)
{
    m_model->AddRef();

    {
        DamageBucketLink*   pdbl;
        while (pdbl = m_damageBuckets.first()) //Intentional =
        {
            delete pdbl->data();
        }
    }

    // You will NOT be notified when a model goes away if you set its mission to NULL first
    if (m_pMission)
        m_pMission->GetIgcSite()->TerminateModelEvent(m_model);

    if (m_cluster)
        m_model->SetCluster(NULL);   //Can be overriden via virtual

    if (m_side)
        m_model->SetSide(NULL);     //ditto

    FreeThingSite();

    m_model->SetMission(NULL);               //Can be overriden.

    m_model->Release();
}

void    CmodelIGC::Update(Time now)
{
    // Model motion now handled by CclusterIGC
    float   dt = (now - m_lastUpdate);
    m_lastUpdate = now;

    if (m_visibleF)
    {
        //
        // set the thing's rotation
        //
        if (m_rotation.angle() != 0.0f)
        {
            if (m_decalF) {
                m_pThingSite->Spin(m_rotation.angle() * dt);
            }
            else
            {
                Orientation o (m_pHitTest->GetOrientation());
                o.PostRotate(m_rotation.axis(), m_rotation.angle() * dt);
                o.Renormalize();

                m_pHitTest->SetOrientation(o);
                m_pThingSite->SetOrientation(o);
            }
        }
    }
}

void   CmodelIGC::LoadCVH(const char*     pszFileName,
                          const char*     pszIconName,
                          ModelAttributes attributes,
                          HitTestShape    htsDefault)
{
    FreeThingSite();

    m_pHitTest = HitTest::Create(pszFileName, m_model, ((attributes & c_mtStatic) != 0), htsDefault);
    ZAssert (m_pHitTest);

    SetMyAttributes(attributes);
    IIgcSite*   pigc = m_pMission->GetIgcSite();
    m_pThingSite = pigc->CreateThingSite(m_model);
    m_poIcon = pigc->LoadRadarIcon(pszIconName);
}

HRESULT   CmodelIGC::LoadEffect(const Color& color, ModelAttributes attributes)
{
    LoadCVH(NULL, NULL, attributes, c_htsSphere);

    return m_pThingSite->LoadEffect(color);
}
HRESULT CmodelIGC::LoadWarp(const char* textureName, const char* iconName, ModelAttributes attributes)
{
    LoadCVH(NULL, iconName, attributes, c_htsCone);

    return m_pThingSite->LoadAleph(textureName);
}

HRESULT CmodelIGC::LoadMine(const char* textureName, float strength, float radius, const char* iconName, ModelAttributes attributes)
{
    LoadCVH(NULL, iconName, attributes, c_htsSphere);

    return m_pThingSite->LoadMine(textureName, strength, radius);
}

HRESULT     CmodelIGC::Load(int                options,
                            const char*        model,
                            const char*        texture,
                            const char*        icon,
                            ModelAttributes    attributes)
{
    LoadCVH(model, icon, attributes);

	HRESULT rc;
	
	// BT - 10/17 - Fixing MoGas crash on model load fail.
	if (m_pThingSite == nullptr)
		rc = E_FAIL;
	else
		rc = m_pThingSite->LoadModel(options, model, texture);

    if (SUCCEEDED(rc))
    {
        m_pThingSite->SetVisible(m_visibleF ? m_render : c_ucRenderNone);
        m_decalF = false;
    }

    return rc;
}

void    CmodelIGC::SetVisibleF(bool visibleF)
{
    if (m_visibleF != visibleF)
    {
        m_visibleF = visibleF;
        m_pThingSite->SetVisible(visibleF ? m_render : c_ucRenderNone);
    }
}

void    CmodelIGC::SetRender(unsigned char render)
{
    m_render = render;
    m_pThingSite->SetVisible(m_visibleF ? render : c_ucRenderNone);
}

void    CmodelIGC::SetPosition(const Vector& newVal)
{
    ZAssert (newVal * newVal >= 0.0f);
    m_pHitTest->SetPosition(newVal);
    m_pThingSite->SetPosition(newVal);
}

void    CmodelIGC::SetOrientation(const Orientation& newVal)
{
    m_pHitTest->SetOrientation(newVal);

    if (m_pThingSite)
        m_pThingSite->SetOrientation(newVal);
}


void    CmodelIGC::SetMyAttributes(ModelAttributes newVal)
{
    ZAssert (m_pHitTest);

    //type also defines the hitTest ID
    switch (newVal & (c_mtHitable | c_mtStatic | c_mtCastRay))
    {
        case c_mtHitable | c_mtStatic:
        {
            //static object ... they don't move & therefore can't collide with other static objects
            m_pHitTest->SetID(c_htidStaticObject);
        }
        break;

        case c_mtCastRay:
        {
            //projectile or treasure ... can collide with any object (static or moving) but
            //not with each other
            m_pHitTest->SetID((HitTestID)NULL);
        }
        break;

        case c_mtHitable | c_mtCastRay:
        {
            //Moving object ... can hit anything with a higher ID (so we don't generate
            //object A collides with object B and B collides with A).
            ZAssert (this != NULL);
            ZAssert (this != c_htidStaticObject);

            m_pHitTest->SetID((HitTestID)this);
        }
        break;

        case c_mtStatic:
        case 0:
        {
            //Do nothing.
        }
        break;

        default:
        {
            ZAssert (false);
        }
    }

    m_attributes = newVal;
}

void    CmodelIGC::SetCluster(IclusterIGC* newVal)
{
    //This method must always be overriden by the wrapper object to, at least, DeleteModel() and AddModel() on the cluster.
    if (m_cluster)
    {
        if (newVal)
        {
            //Special case ... a model is switching from one cluster to another
            //This could be a problem if we are in the middle of collision detection
            //so explicitly flush the old cluster's collision queue.
            m_cluster->FlushCollisionQueue();
        }
        m_cluster->Release();
        m_cluster = NULL;
    }

    m_cluster = newVal;

    if (newVal)
    {
        newVal->AddRef();
    }

    if (m_pThingSite)
    {
        ZAssert (m_pHitTest);
        m_pThingSite->SetCluster(m_model, newVal);
    }
}

HRESULT CmodelIGC::LoadDecal(const char* pszTextureName, const char* iconName, const Color& color, bool bDirectional, float width, ModelAttributes attributes,
                             HitTestShape   htsDefault)
{
    LoadCVH(NULL, iconName, attributes, htsDefault);

    HRESULT rc = m_pThingSite->LoadDecal(pszTextureName, bDirectional, width);

    if (SUCCEEDED(rc))
    {
        m_decalF = true;
    }

    return rc;
}
