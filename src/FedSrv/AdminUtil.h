/*

  Limb's can only have one Sponsor, but a Sponsor can have multiple Limbs.  

  A Limb can be a Sponsor for another Limb.

  If a Sponsor dies (gets destructed), so do all of its Limbs.

  A sponsor that has multiple limbs, does so with multiple inheritance of CAdminSponsor 
  -OR- mulitple containment.   Multiple containment is preferred when the sponsor is
  has a variable number of limbs.
  
*/                                                             


#ifndef DEBUG
#undef markcu
#endif

#ifdef markcu
//#define SHOW_SPONSOR_INFO
#endif

#if defined (SHOW_SPONSOR_INFO) 

#include<typeinfo.h>

#endif

#ifdef SHOW_SPONSOR_INFO



class CDebugClass  // This is a temporary class for Mark2's debugging purposes.
{
public:
   CSimpleArray<CComBSTR> bstrSponsors;
   CSimpleArray<CComBSTR> bstrLimbs;

   ~CDebugClass()
   {
     USES_CONVERSION;
     int s = bstrSponsors.GetSize();                            
     int l = bstrLimbs.GetSize();
     if (s > 0)
     {
       printf("Sponsors left over:\n");
       for(int i = 0; i < s; ++i)
       {          
         printf("%s\n", OLE2A(bstrSponsors[i]));
       }
     }
     if (l > 0)
     {
       printf("Limbs left over:\n");
       for(int i = 0; i < l; ++i)
       {          
         printf("%s\n", OLE2A(bstrLimbs[i]));
       }
     }
     if (s > 0 || l > 0) 
       DebugBreak();
   }
};
extern CDebugClass DebugClass;

#endif


//
// The sole purpose of this class is to provide a 
// connection with the template CAdminSponsor and the class CAdminLimb
//
class CBaseSponsor   
{
public:
  virtual void  DetachLimb() = 0;
};


#ifdef SHOW_SPONSOR_INFO
extern int cSponsor;
extern int cLimb;
#endif



template <class CAdminLimbObj>  // CAdminLimbObj must derive from CAdminLimb
class CAdminSponsor :           // CAdminSponsor is to be inherited by the object "sponsoring" life to a Limb object
  public CBaseSponsor
{
public:

  CAdminSponsor()
  {
#ifdef SHOW_SPONSOR_INFO
    cSponsor++;
    printf (" Spon %i   %s\n", cSponsor, typeid(*this).name());
    DebugClass.bstrSponsors.Add(CComBSTR(typeid(*this).name()));
#endif
    m_pCAdminLimbObj = NULL;
    m_iidInterface = NULL;
  }

  virtual ~CAdminSponsor()
  {
    //
    // If there is a limb attached, destroy it now
    //
    if (m_pCAdminLimbObj)
    {
      m_pCAdminLimbObj->_ForceDestruction();
    }
#ifdef SHOW_SPONSOR_INFO
    int i = DebugClass.bstrSponsors.Find(CComBSTR(typeid(*this).name()));
    if (i == -1) printf("Name not found in array for:\n");
    else DebugClass.bstrSponsors.RemoveAt(i);
    cSponsor--;
    printf ("~Spon %i   %s\n", cSponsor, typeid(*this).name());
#endif
  }

  /*-------------------------------------------------------------------------
   * Make()
   *-------------------------------------------------------------------------
   * Purpose:
   *    
   *  Create a new instance of the Sponsor's limb, if it doesn't already
   *  exist.  Return the result of interface query in **ppInterface.
   */
  HRESULT Make(const IID& iidInterface, void **ppInterface)
  {
    HRESULT hr = S_OK;

    if (m_iidInterface == NULL)
    {
        ZAssert(m_pCAdminLimbObj == NULL);

        RETURN_FAILED(m_pCAdminLimbObj->CreateInstance(&m_pCAdminLimbObj)); // CreateInstance() is static
        
        //
        // Now attach the newly made limb to this
        //
        m_pCAdminLimbObj->SetBaseSponsor(this);
    }
    ZAssert(m_pCAdminLimbObj);

    hr = m_pCAdminLimbObj->QueryInterface(iidInterface, &m_iidInterface);

    *ppInterface = m_iidInterface;

    return hr;
  }

  virtual void DetachLimb()
  {
    m_pCAdminLimbObj = NULL;
    m_iidInterface = NULL;
  }

  CAdminLimbObj* GetLimb() { return m_pCAdminLimbObj; }

private:

  CComObject<CAdminLimbObj>*  m_pCAdminLimbObj;
  void *                      m_iidInterface;
};



template <class CDerivedClass, class TInferfaceClass>  // Class that is inheriting this one (this enables static downcasting)
class CAdminLimb   // this must be inherited by the class given as an argument to the CAdminSponsor template 
{
public:

  CAdminLimb()
  {
#ifdef SHOW_SPONSOR_INFO
    cLimb++;
    printf (" Limb %i   %s\n", cLimb, typeid(*this).name());
    DebugClass.bstrLimbs.Add(CComBSTR(typeid(*this).name()));
#endif
    m_pCBaseSponsor = NULL;
  }

  virtual  ~CAdminLimb()
  {
    DetachFromSponsor();
#ifdef SHOW_SPONSOR_INFO
    int i = DebugClass.bstrLimbs.Find(CComBSTR(typeid(*this).name()));
    if (i == -1) printf("Name not found in array for:\n");
    else DebugClass.bstrLimbs.RemoveAt(i);
    cLimb--;
    printf ("~Limb %i   %s\n", cLimb, typeid(*this).name());
#endif
  }

  void _ForceDestruction()
  {
    DetachFromSponsor();

    //
    // Let remote users know that this object going to be destructed.
    // Downcast to the interface pointer...looks tricky but it avoids using dynamic_cast
    //
    static_cast<CDerivedClass*>(this)->AddRef();
    CoDisconnectObject(static_cast<TInferfaceClass*>((static_cast<CDerivedClass*>(this))), 0); 
    
    // CoDisconnectObject() should have brought the ref count to one, so this line should
    // cause this object to be deleted.
    static_cast<CDerivedClass*>(this)->Release();
  }

  void SetBaseSponsor(CBaseSponsor *pCBaseSponsor) {m_pCBaseSponsor = pCBaseSponsor;}

private:

  void DetachFromSponsor()
  {
    if (m_pCBaseSponsor)
    {
      m_pCBaseSponsor->DetachLimb();
      m_pCBaseSponsor = NULL;
    }
  }

  CBaseSponsor *m_pCBaseSponsor;  
};

