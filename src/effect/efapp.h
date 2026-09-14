#ifndef _efapp_h_
#define _efapp_h_

#include <engineapp.h>
#include <namespace.h>


//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class EffectApp : public EngineApp {
private:
    TRef<INameSpace> m_pns;

public:
    TRef<INameSpace> OptimizeThingGeo(const ZString& str, Geo* pgeo, Number* pnumber);

    //
    // Win32App methods
    //

    HRESULT Initialize(const ZString& strCommandLine, HWND hWindow);
    void Terminate();
};

//////////////////////////////////////////////////////////////////////////////
//
// TrekResources
//
//////////////////////////////////////////////////////////////////////////////

class TrekResources {
public:
    static IEngineFont* SmallFont();
    static IEngineFont* SmallBoldFont();
    static IEngineFont* LargeFont();
    static IEngineFont* LargeBoldFont();
    static IEngineFont* HugeFont();
    static IEngineFont* HugeBoldFont();

    static COLORREF WhiteColor() { return RGB(255,255,255); }
    static COLORREF BlackColor() { return RGB(0,0,0); }
    static COLORREF CyanColor() { return RGB(117,217,210); }
    static COLORREF YellowColor() { return RGB(207,255,55); }
    static COLORREF GridTextColor() { return RGB(117,217,210); }
    static COLORREF GridSelTextColor() { return RGB(0,0,0); }

    

    static void Initialize(Modeler* pmodeler);
    static IEngineFont* GetFont(int fontSize, bool bold);
    static void Terminate();
};

#endif
