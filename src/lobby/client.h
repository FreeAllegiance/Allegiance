/*-------------------------------------------------------------------------
  client.h
  
  Per client stuff on lobby side
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#ifndef _CLIENT_H_
#define _CLIENT_H_

// There shld be a one-to-one mapping between connections on the servers session and CFLServer objects, 
// but there's not, since each connection can (for now) actually support more than one game). 
// CFLServer is more of a server "game" than a server "connection"

class CFLClient : public IObject
{
public:
  CFLClient(CFMConnection * pcnxn) :
  m_dwID(c_dwID),
  m_pcnxn(pcnxn)
  {
    ZAssert(pcnxn);
    m_pcnxn->SetPrivateData((DWORD) this); // set up two-way link between connection and this
  }
  ~CFLClient()
  {
    m_pcnxn->SetPrivateData(0); // disconnect two-way link between connection and this
  }
  bool IsValidThisPtr() // this
  {
    bool fValid = this && (c_dwID == m_dwID);
    ZAssert (fValid); // this should only be false if either a bug in the client, or hacked client
    return fValid;
  }
  static CFLClient * FromConnection(CFMConnection & cnxn) 
  {
    return (CFLClient *)cnxn.GetPrivateData();
  }
  CFMConnection * GetConnection()
  {
    return m_pcnxn;
  }

private:
  static const DWORD c_dwID;
  static const int c_cMaxPlayers;
  CFMConnection * m_pcnxn;
  DWORD           m_dwID;
};

// BT - 9/11/2010 - Making this usable by other classes.
void encodeURL( char * url, char * token);

#endif

