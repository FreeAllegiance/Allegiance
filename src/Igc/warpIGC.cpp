/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	warpIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CwarpIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// warpIGC.cpp : Implementation of CwarpIGC
#include "warpIGC.h"
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////
// CwarpIGC
HRESULT     CwarpIGC::Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
{
    TmodelIGC<IwarpIGC>::Initialize(pMission, now, data, dataSize);

    ZRetailAssert (data && (dataSize == sizeof(DataWarpIGC)));
    {
        DataWarpIGC*  dataWarp = (DataWarpIGC*)data;
        m_warpDef = dataWarp->warpDef;

        IclusterIGC*    cluster = pMission->GetCluster(dataWarp->clusterID);
        ZRetailAssert (cluster);
        {
            //  , use the real texture name
            HRESULT rc = LoadWarp(dataWarp->warpDef.textureName,
                                  dataWarp->warpDef.iconName,
                                  c_mtStatic | c_mtHitable | c_mtSeenBySide | c_mtPredictable);

            ZAssert (SUCCEEDED(rc));
            {
                SetRadius((float)m_warpDef.radius);

                SetPosition(dataWarp->position);
                SetVelocity(Vector::GetZero());

                {
                    Orientation o(dataWarp->forward);
                    SetOrientation(o);
                }
                SetRotation(dataWarp->rotation);
                SetCluster(cluster);

                SetMass(0.0f);
				// KG- hack for unmovable alephs (to avoid a IGC file format change)
				// aleph name with a leading '*' denotes a fixed position aleph
				// so remove the '*' from the name and set m_bFixedPosition to true
				// Andon: Added mass limits to alephs. It checks for a leading '+'
				// and if found it searches for a ':' - It then takes everything after
				// this and turns it into a number. After that, it replaces the ':' with
				// a '(' and adds a ')' to the end.
				if (dataWarp->name[0] == '*')
				{
					m_bFixedPosition = true;
					if (dataWarp->name[1] == '+')//Andon: Added for if the aleph is both unmoved and mass limited
					{
						ZString name = dataWarp->name;
						int MassFind = name.Find(':',0); 
						char* mass = &(dataWarp->name[MassFind+2]); 
						m_MassLimit = atoi(mass);
						name.ReplaceAll(":" ,'('); //Replaces the : with a (
						const char* alephname = &(name[2]); //Removes the leading '*+'
						char* newAlephName = new char[25];
						const char* nameSuffix = ")"; //Adds a ) to the end
						strncpy(newAlephName, alephname, strlen(alephname)+1);
						strncat(newAlephName, nameSuffix, strlen(nameSuffix)+1);
						SetName(newAlephName);
					}
					else
					{
						SetName(&(dataWarp->name[1])); // skip the leading '*'
						m_MassLimit = -1;
					}
				}
				else if (dataWarp->name[0] == '+')
				{
					ZString name = dataWarp->name;
					int MassFind = name.Find(':',0);
					char* mass = &(dataWarp->name[MassFind+1]);
					m_MassLimit = atoi(mass);
					name.ReplaceAll(":" ,'('); //Replaces the : with a (
					const char* alephname = &(name[1]);//Skip the leading '+'
					char* newAlephName = new char[25];
					const char* nameSuffix = ")"; //Adds a ) to the end
					strncpy(newAlephName, alephname, strlen(alephname)+1);
					strncat(newAlephName, nameSuffix, strlen(nameSuffix)+1);
					SetName(newAlephName);
				}
				else
				{
                	SetName(dataWarp->name);
					m_MassLimit = -1;
				}

                SetSignature(dataWarp->signature);

                pMission->AddWarp(this);
            }
        }
    }

    return S_OK;
}

int         CwarpIGC::Export(void* data) const
{
    if (data)
    {
        DataWarpIGC*  dataWarp = (DataWarpIGC*)data;
        dataWarp->warpDef = m_warpDef;

        dataWarp->position = GetPosition();
        dataWarp->forward = GetOrientation().GetForward();
        dataWarp->rotation = GetRotation();

        ZAssert (GetCluster());
        dataWarp->clusterID = GetCluster()->GetObjectID();
        UTL::putName(dataWarp->name, GetName());
        dataWarp->signature = GetSignature();
    }

    return sizeof(DataWarpIGC);
}
