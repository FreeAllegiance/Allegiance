#include "engineapp.h"

#include <token.h>

#include "engine.h"
#include "enginep.h"
#include "inputengine.h"
#include "model.h"
#include "pane.h"
#include "popup.h"

//////////////////////////////////////////////////////////////////////////////
//
// Engine App
//
//////////////////////////////////////////////////////////////////////////////

HRESULT EngineApp::Initialize(const ZString& strCommandLine, HWND hWindow )
{
    //
    // Initialize the pane code
    //

    Pane::Initialize();

    //
    // Parse the command line for device options
    //

    bool bSoftwareOnly;
    bool bPrimaryOnly;
    DWORD dwBPP = 0; // KGJV 32B - forced bpp, 16, 32 or 0 = use desktop
    ParseCommandLine(strCommandLine, bSoftwareOnly, bPrimaryOnly, dwBPP);

    //
    // Create the engine
    m_pengine = CreateEngine(!bSoftwareOnly, !bPrimaryOnly, dwBPP, hWindow );

    if (!m_pengine->IsValid()) {
        return E_FAIL;
    }

    //
    // Create the modeler
    //

    m_pmodeler = Modeler::Create(m_pengine);

    //
    // Create the popup container
    //

    m_ppopupContainer = CreatePopupContainer();

    return S_OK;
}

void EngineApp::Terminate()
{
    if (m_pmodeler)
        m_pmodeler->Terminate();
    if (m_pengine)
        m_pengine->TerminateEngine( true );

    m_pmodeler = NULL;
    m_pengine = NULL;

    Win32App::Terminate();
}

void EngineApp::ParseCommandLine(
    const ZString& strCommandLine,
    bool& bSoftwareOnly,
    bool& bPrimaryOnly,
    DWORD& dwBPP
) {
    bPrimaryOnly     = false;
    bSoftwareOnly    = false;

    PCC pcc = strCommandLine;
    CommandLineToken token(pcc, strCommandLine.GetLength());

    while (token.MoreTokens()) {
        ZString str;
        if (token.IsMinus(str)) {
            if (str == "primary") {
                bPrimaryOnly = true;
            } else if (str == "software") {
                bSoftwareOnly = true;
            }
            //KGJV 32B
            if (str == "32b") {
                dwBPP = 32;
            }
            else if (str == "16b") {
                dwBPP = 16;
            }
        } else {
            token.IsString(str);
        }
    }
}

void EngineApp::SetInput(InputEngine* pinput)
{
    m_pinput = pinput;
}

int EngineApp::OnException(DWORD code, ExceptionData* pdata)
{
    if (m_pengine && m_pengine->IsFullscreen()) {
        m_pengine->DebugSetWindowed();
    }

    if (m_pinput) {
        MouseInputStream* pmouse = m_pinput->GetMouse();
        if (pmouse) {
            pmouse->SetEnabled(false);
        }
    }

    return EXCEPTION_CONTINUE_SEARCH;
}

#ifdef _DEBUG
    bool EngineApp::OnAssert(const char* psz, const char* pszFile, int line, const char* pszModule)
    {
        OnException(0, NULL);
        return Win32App::OnAssert(psz, pszFile, line, pszModule);
    }
#endif

// KGJV added for Win32App behavior
bool EngineApp::IsBuildDX9()
{
	return true;
}
