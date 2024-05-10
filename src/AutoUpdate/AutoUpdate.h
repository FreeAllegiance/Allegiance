// AllSrvUpdate.h : Declaration of the CAutoUpdate

#pragma once

#include "resource.h"       // main symbols
#include <atlhost.h>

/////////////////////////////////////////////////////////////////////////////
// CAutoUpdate
class CAutoUpdate : 
    public CDialogImpl<CAutoUpdate>
//	public CSimpleDialog<IDD_ALLSRVUPDATE>
{
// Message Map
public:
    BEGIN_MSG_MAP(CAutoUpdate)
	    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	    MESSAGE_HANDLER(WM_DESTROY, OnDestroyWindow)
	    COMMAND_ID_HANDLER(IDOK, OnOK)
	    COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
    END_MSG_MAP()

// Construction / Destruction
public:
    CAutoUpdate() :
        m_bUpdatingService(false),
        m_nExitCode(1)
	{
     m_szPostUpdateEXE[0] = 0;
	}

	~CAutoUpdate()
	{
	}

// Operations
public:

    int  Init(int argc, char* argv[]);
    int  Run();

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
        m_ProgressFileList = GetDlgItem(IDC_PROGRESS_FILELIST);
        m_ProgressAnalyze  = GetDlgItem(IDC_PROGRESS_ANALYZE );
        m_ProgressDownload = GetDlgItem(IDC_PROGRESS_DOWNLOAD);
        m_staticFileName   = GetDlgItem(IDC_STATIC_FILENAME  );

        m_staticFileName.SetWindowText("");

		return 1;  // Let the system set the focus
	}

	LRESULT OnDestroyWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
        PostQuitMessage(m_nExitCode);
		return 0;
	}

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
          m_pAutoDownload->Abort();
//        DestroyWindow();
//        PostQuitMessage(1);
//        m_nExitCode = 0;
//        DestroyWindow();
		//EndDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
        m_pAutoDownload->Abort();
//          m_pAutoDownload->Abort();
//        DestroyWindow();
//        PostQuitMessage(1);
//        m_nExitCode = 1;
//        DestroyWindow();
		//EndDialog(wID);
		return 0;
	}

// Enumerations
public:
    enum {IDD = IDD_AUTOUPDATE};
    CWindow m_ProgressFileList;
    CWindow m_ProgressAnalyze;
    CWindow m_ProgressDownload;
    CWindow m_staticFileName;
    char m_szPostUpdateEXE[MAX_PATH];

// Data Members
protected:
    int m_nExitCode;
    bool m_bUpdatingService;
    IAutoDownload * m_pAutoDownload;
};


/////////////////////////////////////////////////////////////////////////////