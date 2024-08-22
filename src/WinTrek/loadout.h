/*==========================================================================
 *
 *  Copyright (C) 1997,1998 Microsoft Corporation. All Rights Reserved.
 *
 *  File:  TrekCtrls.h
 *
 *  Author: 
 *
 ***************************************************************************/

#pragma once

#include"trekmdl.h"

/////////////////////////////////////////////////////////////////////////////
//
// PartInfoPane
//
/////////////////////////////////////////////////////////////////////////////

class ConnectorPane : public ValuePane 
{
public:
    static TRef<ConnectorPane> Create();
    virtual void SetVisible(bool bVisible) = 0;
};

class PartInfoPane : public ValuePane 
{
public:
    static TRef<PartInfoPane> Create(ConnectorPane* pConnectorPanePurchases, ConnectorPane* pConnectorPaneInventory);

    virtual void DisplayTeamPurchase(IbucketIGC* pBucket, bool bPartInInventory) = 0;
    virtual void DisplayNothing() = 0;
};

/////////////////////////////////////////////////////////////////////////////
//
// PurchasesPane
//
/////////////////////////////////////////////////////////////////////////////

class PurchasesPane : public ValuePane 
{
public:
    static TRef<PurchasesPane> Create(PartInfoPane* pPartInfoPane);
    virtual  void SetPaneWindow(Window* pWindow) = 0;
};