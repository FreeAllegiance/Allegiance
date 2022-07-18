#ifndef _engineapp_h_
#define _engineapp_h_

#include "popup.h"
#include "enginewindow.h"

#include <Win32app.h>

//////////////////////////////////////////////////////////////////////////////
//
// EngineApp
//
//////////////////////////////////////////////////////////////////////////////
class Engine;
class Modeler;
class InputEngine;

class EngineApp : public Win32App {
private:
    TRef<Engine>           m_pengine;
    TRef<Modeler>          m_pmodeler;
    TRef<IPopupContainer>  m_ppopupContainer;
    TRef<InputEngine> m_pinput;
    TRef<EngineWindow> m_pengineWindow;

    void ParseCommandLine(
        const ZString& strCommandLine,
        bool& bSoftwareOnly,
        bool& bPrimaryOnly,
        DWORD& dwBPP
    );

public:
    //
    // EngineApp methods
    //

    Engine*          GetEngine()         { return m_pengine;         }
    Modeler*         GetModeler()        { return m_pmodeler;        }
    IPopupContainer* GetPopupContainer() { return m_ppopupContainer; }
    EngineWindow*    GetEngineWindow() { return m_pengineWindow; }

    void SetInput(InputEngine* pinput);
    void SetEngineWindow(EngineWindow* pengineWindow) {
        m_pengineWindow = pengineWindow;
    }

    //
    // Win32App methods
    //

    HRESULT Initialize(const ZString& strCommandLine, HWND hWindow);
    void Terminate();

    int OnException(DWORD code, ExceptionData* pdata);

    #ifdef _DEBUG
        bool OnAssert(const char* psz, const char* pszFile, int line, const char* pszModule);
    #endif

	bool IsBuildDX9(); // KGJV added - see ZLib/Win32App
};

#endif
