/*==========================================================================
 *
 *  Copyright (C) 1997,1998 Microsoft Corporation. All Rights Reserved.
 *
 *  File:  TrekDlgs.h
 *
 *  Author: 
 *
 ***************************************************************************/

#pragma once

#include "loadout.h"
#include "trekctrls.h"

// Forward Declarations
class TrekDialog;


//////////////////////////////////////////////////////////////////////////////
//
// TrekControlWindow
//
//////////////////////////////////////////////////////////////////////////////

class TrekControlWindow : public ChildWindow
{
public:
    TrekControlWindow(){ m_fEnabled = true; }
    virtual BOOL OnParentNotify(UINT code, LPARAM lParam, LRESULT* pResult);
    virtual void OnParentDrawItem(LPDRAWITEMSTRUCT lpdis);
    void EnableWindow(bool fEnable = true)
        { m_fEnabled = fEnable; ::EnableWindow(GetHWND(), fEnable); }
protected:
	bool m_fEnabled;
};

//////////////////////////////////////////////////////////////////////////////
//
// TrekScrollBar
//
//////////////////////////////////////////////////////////////////////////////

class TrekScrollBar : public PaneWindow {
private:
    TRef<IIntegerEventSource> m_pevent;
    TRef<ScrollPane> m_pscrollPane;
    
public:
    TrekScrollBar(Window* pparent, unsigned height, Modeler* pmodeler):
        PaneWindow(pparent, pmodeler->GetEngine())
    {
        SetPane(CreateTrekScrollPane(height, pmodeler, m_pevent, m_pscrollPane));
    }

    void SetPageSize(int sizePage)
    {
        m_pscrollPane->SetPageSize(sizePage);
    }

    void SetSize(int size)
    {
        m_pscrollPane->SetSize(size);
    }

    void SetPos(int pos)
    {
        m_pscrollPane->SetPos(pos);
    }

    IIntegerEventSource* GetEventSource()
    {
        return m_pevent;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// TrekImgButton
//
//////////////////////////////////////////////////////////////////////////////

class TrekImgButton : public TrekControlWindow 
{
public:
	TrekImgButton();
	~TrekImgButton();
	
    BOOL Create(TrekDialog* pwindowParent, const WinPoint& ptOrigin, const WinPoint& ptSize,
        LPCSTR szImg, UINT nID, bool fTabStyle = false);

    virtual void OnPaint(HDC hdc, const WinRect& rect);
	virtual bool OnMouseMessage(UINT message, UINT nFlags, const WinPoint& point);

    void EnableWindow(bool fEnable = true)
        {  TrekControlWindow::EnableWindow(fEnable);
        	InvalidateRect(GetHWND(), NULL, TRUE); }

	void Check( bool fChecked )
		{ m_fChecked = fChecked; InvalidateRect(GetHWND(), NULL, TRUE); }
	bool IsChecked()
		{ return m_fChecked; }


protected:
	bool 		m_fButtonDown;
	bool 		m_fMouseOver;
	bool 		m_fChecked;
	bool 		m_fTabStyle;
	
    TRef<Surface> m_pSurface;
};

//////////////////////////////////////////////////////////////////////////////
//
// TrekButton
//
//////////////////////////////////////////////////////////////////////////////

class TrekButton : public TrekControlWindow 
{
public:
    TrekButton();
    ~TrekButton();
    BOOL Create(TrekDialog* pwindowParent, const WinRect& rect, 
        LPCSTR szSelected, LPCSTR szFocus, LPCSTR szDisabled, UINT nID, Style style = StyleBSPushButton());
    virtual void OnParentDrawItem(LPDRAWITEMSTRUCT lpdis);

protected:
    virtual DWORD WndProc(UINT, WPARAM, LPARAM);

protected:
    WinPoint m_ptImgOrigin;
    HBITMAP m_hbmpSelected;
    HBITMAP m_hbmpFocus;
    HBITMAP m_hbmpDisabled;
};

//////////////////////////////////////////////////////////////////////////////
//
// TrekCheckBox
//
//////////////////////////////////////////////////////////////////////////////

class TrekCheckBox : public TrekButton
{
public:
	TrekCheckBox() 
		{ m_fChecked = false; }
	void Check( bool fChecked )
		{ m_fChecked = fChecked; InvalidateRect(GetHWND(), NULL, TRUE); }
	bool IsChecked()
		{ return m_fChecked; }

    virtual void OnParentDrawItem(LPDRAWITEMSTRUCT lpdis);
	
protected:

	bool m_fChecked;
};

//////////////////////////////////////////////////////////////////////////////
//
// TrekEdit
//
//////////////////////////////////////////////////////////////////////////////

class TrekEdit : public TrekControlWindow 
{
public:
    TrekEdit();
    ~TrekEdit();
    BOOL Create(TrekDialog* pwindowParent, const WinRect& rect, UINT nID);

protected:
    virtual DWORD WndProc(UINT, WPARAM, LPARAM);

};

//////////////////////////////////////////////////////////////////////////////
//
// TrekImgCtrl
//
//////////////////////////////////////////////////////////////////////////////

class TrekImgCtrl : public TrekControlWindow 
{
public:
    TrekImgCtrl();
    ~TrekImgCtrl();
    BOOL Create(Window* pwindowParent, const WinRect& rect, 
    			LPCSTR szImg, HFONT hfont, COLORREF color, long lFlags, UINT nID);
    virtual void OnPaint(HDC hdc, const WinRect& rect);
    void SetText(LPCSTR szText);

protected:
	COLORREF m_color;
	HFONT m_hfont;
	long m_lFlags;
	WinPoint m_ptOffset;
	TRef<Surface> m_pSurface;
	ZString m_strText;
};

//////////////////////////////////////////////////////////////////////////////
//
// TrekHatchCtrl
//
//////////////////////////////////////////////////////////////////////////////
class TrekHatchCtrl;

class TrekHatchNotify
{
public:
    virtual void OnHatchClosed(TrekHatchCtrl* pHatch) = 0;
};

class TrekHatchCtrl : public TrekControlWindow
{
public:
    TrekHatchCtrl();
    ~TrekHatchCtrl();

    BOOL Create(Window* pwindowParent, const WinRect& rect, int ySplit, LPCSTR szImgHatch, UINT nID);
    virtual void OnPaint(HDC hdc, const WinRect& rect);

	void Close(LPCSTR szImgSurfaceExposed, TrekHatchCtrl* pHatchToOpenNext, TrekHatchNotify* pNotify, LPCSTR szImgToOpenNext);
	void Open(LPCSTR szImgSurfaceExposed);
	void OpenNext();

   	virtual bool OnTimer();
   	void StartTimer();
	void StopTimer();

    virtual DWORD WndProc(UINT, WPARAM, LPARAM);

protected:

	int m_ySplit;
	int m_nPercentOpen;
	int m_nRate;
	TRef<IEventSink>	m_pEventSink;
	TRef<Surface> 		m_pSurfaceHatch;
	TRef<Surface> 		m_pSurfaceExposed;
	TrekHatchCtrl*		m_pHatchToOpenNext;
	ZString				m_strImgToOpenNext;
	TrekHatchNotify*    m_pNotify;
};


//////////////////////////////////////////////////////////////////////////////
//
// TrekGeoCtrl
//
//////////////////////////////////////////////////////////////////////////////

class TrekGeoCtrl :
    public TrekControlWindow,
    public IEventSink
{
public:
    TrekGeoCtrl();
    ~TrekGeoCtrl();
    BOOL Create(Window* pwindowParent, const WinRect& rect, 
            HBITMAP hbmpBkgnd, const WinPoint& ptImgOrigin, UINT nID);
    virtual void OnPaint(HDC hdc, const WinRect& rect);
    void SetGeo(IshipIGC* pship);
    void SetGeo(LPCSTR szModel);
    
    bool OnEvent(IEventSource* pevent);

protected:

    float            m_radius;
    float            m_distance;
    float            m_angleYaw;
    TRef<GeoImage>   m_pGeoImage;
    TRef<Viewport>   m_pviewport;
    TRef<Camera>     m_pcamera;
    TRef<ModifiableRectValue> m_prectView;
	TRef<RotateTransform> m_protate;
    TRef<IEventSink> m_peventSink;
    HBITMAP          m_hbmpBkgnd;
    WinPoint         m_ptImgOrigin;
    TRef<ThingSite>  m_pThingSite;
};

//////////////////////////////////////////////////////////////////////////////
//
// TrekGridCells
//
//////////////////////////////////////////////////////////////////////////////

class TrekGridCell : public IObjectSingle
{
public:
    virtual void DrawCell(HDC hdc, const WinRect& rect, bool fSelected) = 0;
};

class TrekTextCell : public TrekGridCell
{
public:
    TrekTextCell(LPCSTR szCellText, short nInset = 4)
        { m_strCellText = szCellText; m_nInset = nInset;}
    TrekTextCell(int nCellText, short nInset = 4);

    virtual void DrawCell(HDC hdc, const WinRect& rect, bool fSelected);

protected:
    ZString m_strCellText;    
    short m_nInset;
};

//////////////////////////////////////////////////////////////////////////////
//
// TrekGridRow
//
//////////////////////////////////////////////////////////////////////////////

class TrekGridRow : public IObjectSingle
{
public:
    TrekGridRow(int nNumCols, long lRowData)
		{ m_lRowData = lRowData; m_vCells.SetCount(nNumCols); }
	
    long GetRowData() 
        { return m_lRowData; }
    TrekGridCell* GetCell(int iCol) 
        { return m_vCells[iCol]; }
    void SetCell(int iCol, TrekGridCell* pCell)
        { m_vCells.Set(iCol, pCell); }

protected:
    long m_lRowData;
    TVector<TRef<TrekGridCell> > m_vCells;
};

//////////////////////////////////////////////////////////////////////////////
//
// TrekGridRow
//
//////////////////////////////////////////////////////////////////////////////

class TrekGrid : public TrekControlWindow, public IIntegerEventSink 
{
public:
    TrekGrid();
    ~TrekGrid();
    BOOL Create(Window* pwindowParent, const WinRect& rect,
            HBITMAP hbmpSel, int nNumCols, int nRowHeight, bool fScrollBar, UINT nID);

    void InsertRow(int iRow, long lRowData);
    int AppendRow(long lRowData);
    void DeleteRow(int iRow);
    void DeleteAllRows();    

    long GetRowData(int iRow);
    int FindRow(long lRowData);
    int GetRowCount() { return m_vRows.GetCount(); }
    
    void SetCellText(int iRow, int iCol, LPCSTR szCellText, short nInset = 4);
    void SetCellText(int iRow, int iCol, int nCellText, short nInset = 4);
    void SetCell(int iRow, int iCol, TrekGridCell* pCell);
    TrekGridCell* GetCell(int iRow, int iCol);

    void SetColWidth(int iCol, int nColWidth);
    void SetSelRow(int iRow);
    int GetSelRow();
    void UpdateAll();

    virtual bool OnEvent(IIntegerEventSource* pevent, int value);

protected:
    virtual void DrawRow(HDC hdc, int iRow, const WinRect& rect);
    virtual void OnPaint(HDC hdc, const WinRect& rect);
    virtual bool OnKey(const KeyState& ks);
    virtual bool OnMouseMessage(UINT message, UINT nFlags, const WinPoint& point);
    void PointToCell(const WinPoint& point, int* piRow, int *piCol);
    int LastVisibleRow()
        { return m_iTopRow + m_cVisibleRows - 1; }

    
protected:
    int m_iSelRow;
    int m_iTopRow;
    int m_cVisibleRows;
    int m_nRowHeight;
    int m_nNumCols;
    HBITMAP m_hbmpSel;  
    HBITMAP m_hbmpBkgnd;  
    HBITMAP m_hbmpRightEdge;  
    WinPoint m_ptImgOrigin;

    TRef<TrekScrollBar> m_pScrollBar;
    TRef<IIntegerEventSink> m_psinkScrollBar;
    TVector<TRef<TrekGridRow> > m_vRows;
    TVector<int> m_vColWidths;
};

//////////////////////////////////////////////////////////////////////////////
//
// TrekDialog
//
//////////////////////////////////////////////////////////////////////////////

class TrekDialog : public Dialog, public IClientEventSink
{
public:
    TrekDialog(Window* pwndParent, short nAmbientSound, int nResourceID);
    ~TrekDialog();

	// creation & destruction
    virtual BOOL Create();
    virtual void Destroy();

	// accessors
    HBITMAP    GetBkgrnd()
        { return m_hbmpBkgrnd; }
	
	// msg handling
    virtual DWORD WndProc(UINT message, WPARAM wParam, LPARAM lParam);

protected:

	// msg handling
    virtual void OnPaint(HDC hdc, const WinRect& rect);
    virtual BOOL OnNotify(HWND hwndFrom, UINT idFrom, UINT code, LPARAM lParam, LRESULT* pResult);
    virtual void OnDrawItem(UINT nCtrlID, LPDRAWITEMSTRUCT lpdis);

    HBITMAP    			m_hbmpBkgrnd;
    HBITMAP             m_hbmpBkgrnd3D;
	short				m_nAmbientSound;
    HBRUSH              m_hbrEditControl; 

    TRef<IClientEventSink>      m_pClientEventSink;
    TRef<IClientEventSource>    m_pClientEventSource;
};

//////////////////////////////////////////////////////////////////////////////
//
// MainDlg
//
//////////////////////////////////////////////////////////////////////////////

class MainDlg : public TrekDialog
{
public:
    MainDlg(Window* pwndParent);

    #ifdef _DEBUG
        DWORD __stdcall AddRef();
        DWORD __stdcall Release();
    #endif

protected:

    virtual bool OnCommand(WPARAM wParam, LPARAM lParam);

    // message handling
    virtual BOOL OnInitDialog();

protected:

    TrekButton          m_btnMissions;
    TrekButton          m_btnOptions;
    TrekButton          m_btnExit;
    TrekButton          m_btnProfile;
    TrekButton          m_btnRanking;
};



//////////////////////////////////////////////////////////////////////////////
//
// MissionsDlg
//
//////////////////////////////////////////////////////////////////////////////

class MissionsDlg : public TrekDialog,  public EventTargetContainer<MissionsDlg>
{
public:
    MissionsDlg(Window* pwndParent);

    // mission updates
    virtual void OnAddMission(MissionInfo* pMissionInfo);
    virtual void OnDelMission(MissionInfo* pMissionInfo);

    // team updates
    virtual void OnAddPlayer(MissionInfo* pMissionInfo, SideID sideID, PlayerInfo* pPlayerInfo);
    virtual void OnDelPlayer(MissionInfo* pMissionInfo, SideID sideID, PlayerInfo* pPlayerInfo, QuitSideReason reason);
    virtual void OnTeamInactive(MissionInfo* pMissionInfo, SideID sideID);
    virtual void OnDelRequest(MissionInfo* pMissionInfo, SideID sideID, PlayerInfo* pPlayerInfo, DelPositionReqReason reason);

protected:

    bool OnPickCiv(int civID);
    bool OnQuitTeam();

    virtual bool OnCommand(WPARAM wParam, LPARAM lParam);
	virtual bool OnTimer();

    // selection changes
    void OnSelectMission(DWORD missionSel);
    void OnSelectSide(SideID sideIDSel);

    // message handling
    virtual BOOL OnInitDialog();

    void Logon();
    BOOL PopulateDialog(bool fAddMissions);
	void UpdateRoster(MissionInfo* pMissionInfo, SideID sideID, PlayerInfo* pPlayerInfo, bool fAddPlayer);

protected:
	MissionInfo*		m_pMissionInfoSel;
    SideID              m_sideIDSel;
    bool                m_bInitializing;
    
    TrekImgButton       m_btnNewMission;
    TrekImgButton       m_btnMain;
    TrekImgButton       m_btnReqPos;
    TrekGrid            m_gridMissions;
    TrekGrid            m_gridTeams;
    TrekGrid            m_gridPlayers;
    TrekImgCtrl         m_wndMissionDesc;
};


//////////////////////////////////////////////////////////////////////////////
//
// SideStrategyDlg
//
//////////////////////////////////////////////////////////////////////////////

class SideStrategyDlg : public TrekDialog
{
public:
    SideStrategyDlg(Window* pwndParent);

    // notifications
    virtual void OnTeamReadyChange(MissionInfo* pMissionDef, SideID sideID, bool fTeamReady);
    virtual void OnTeamInactive(MissionInfo* pMissionInfo, SideID sideID);
    virtual void OnAddPlayer(MissionInfo* pMissionInfo, SideID sideID, PlayerInfo* pPlayerInfo);
    virtual void OnDelPlayer(MissionInfo* pMissionInfo, SideID sideID, PlayerInfo* pPlayerInfo, QuitSideReason reason);
	virtual void OnAddRequest(MissionInfo* pMissionInfo, SideID sideID, PlayerInfo* pPlayerInfo);
	virtual void OnDelRequest(MissionInfo* pMissionInfo, SideID sideID, PlayerInfo* pPlayerInfo, DelPositionReqReason reason);
    virtual void OnMoneyChange(PlayerInfo* pPlayerInfo);
	virtual void OnPlayerStatusChange(MissionInfo* pMissionInfo, SideID sideID, PlayerInfo* pPlayerInfo);
	virtual void OnTeamAutoAcceptChange(MissionInfo* pMissionInfo, SideID sideID, bool fAutoAccept);
	virtual void OnTeamForceReadyChange(MissionInfo* pMissionInfo, SideID sideID, bool fReady);
    virtual void OnNewChatMessage(void);
    virtual void OnMissionStarted(MissionInfo* pMissionInfo);
	virtual void OnStationCaptured(StationID stationID, SideID sideID);

protected:

    virtual bool OnCommand(WPARAM wParam, LPARAM lParam);
    virtual BOOL OnInitDialog();
    
	void SendPosAck(bool fAccepted);
    void UpdateButtonStates();
    void UpdateAcceptRejectButtons();
    void UpdateOtherTeamStatus(SideID sideID);
    void UpdatePlayerInfo(PlayerInfo* pPlayerInfo, bool fRequest, bool fAddPlayer);
    LPCSTR StatusForMember(PlayerInfo* pPlayerInfo, bool fRequest);
    void OnSelectPlayer();


protected:
    PlayerInfo*		m_pPlayerInfoMe;
    MissionInfo*		m_pMissionInfoMe;
    SideID				m_sideIDMe;
    bool                m_fDisabled;

    TrekImgButton       m_btnLoadout;
    TrekImgButton       m_btnAccept;
    TrekImgButton       m_btnReject;
    TrekImgButton       m_btnQuit;
    TrekButton          m_btnChatSend;
	TrekImgButton		m_chkAutoAccept;
	TrekImgButton		m_chkSideReady;
	TrekImgButton		m_chkImReady;
    TrekGrid            m_gridChat;
    TrekGrid            m_gridPlayers;
    TrekGrid            m_gridOtherTeams;
    TrekImgCtrl         m_wndMissionDesc;
    TrekImgCtrl         m_wndStationName;
    TrekImgCtrl         m_wndTeamName;
    TrekImgCtrl         m_wndTeamIcon;
    TrekEdit            m_editChat;

};

//////////////////////////////////////////////////////////////////////////////
//
// GameOverDlg
//
//////////////////////////////////////////////////////////////////////////////

class GameOverDlg : public TrekDialog
{
public:
    GameOverDlg(Window* pwndParent);

    // notifications
    virtual void OnAddPlayer(MissionInfo* pMissionInfo, SideID sideID, PlayerInfo* pPlayerInfo);
    virtual void OnDelPlayer(MissionInfo* pMissionInfo, SideID sideID, PlayerInfo* pPlayerInfo, QuitSideReason reason);
	virtual void OnPlayerStatusChange(MissionInfo* pMissionInfo, SideID sideID, PlayerInfo* pPlayerInfo);
    virtual void OnNewChatMessage(void);

protected:

    virtual bool OnCommand(WPARAM wParam, LPARAM lParam);
    virtual BOOL OnInitDialog();
    
    void UpdateButtonStates();
    void UpdatePlayerInfo(PlayerInfo* pPlayerInfo);


protected:
    PlayerInfo*		    m_pPlayerInfoMe;
    MissionInfo*		m_pMissionInfoMe;
    SideID				m_sideIDMe;
    bool                m_fDisabled;

    TrekImgButton       m_btnQuit;
    TrekGrid            m_gridChat;
    TrekGrid            m_gridPlayers;
    TrekEdit            m_editChat;

};


//////////////////////////////////////////////////////////////////////////////
//
// LoadoutDlg
//
//////////////////////////////////////////////////////////////////////////////


class LoadoutDlg : public TrekDialog
{
public:
    LoadoutDlg(Window* pwndParent);
    ~LoadoutDlg(void);

	// view notifications
	virtual void OnMissionStarted(MissionInfo* pMissionInfo);
    virtual void OnNewChatMessage(void);
	virtual void OnStationCaptured(StationID stationID, SideID sideID);
    virtual void OnDelPlayer(MissionInfo* pMissionInfo, SideID sideID, PlayerInfo* pPlayerInfo, QuitSideReason reason);

protected:

    virtual bool OnCommand(WPARAM wParam, LPARAM lParam);

    // message handling
    virtual BOOL OnInitDialog();


private:
    TRef<ConnectorPane> m_pConnectorPanePurchases;
    TRef<PaneWindow>    m_pPurchasesConnectorWindow;
    TRef<ConnectorPane> m_pConnectorPaneInventory;
    TRef<PaneWindow>    m_pInventoryConnectorWindow;
    TRef<CreditsPane>   m_pCreditsPane;
    TRef<PaneWindow>    m_pCreditsWindow;
    TRef<PurchasesPane> m_pPurchasesPane;
    TRef<PaneWindow>    m_pPurchasesWindow;
    TRef<InventoryPane> m_pInventoryPane;
    TRef<PaneWindow>    m_pInventoryWindow;
    TRef<PartInfoPane>  m_pPartInfoPane;
    TRef<PaneWindow>    m_pPartInfoWindow;

    PlayerInfo*		    m_pPlayerInfoMe;
    MissionInfo*		m_pMissionInfoMe;
    SideID				m_sideIDMe;
    
    TrekImgButton       m_btnTeamRoom;
    TrekImgButton       m_btnLaunch;

	TrekGrid            m_gridChat;
    TrekEdit            m_editChat;
	TrekButton          m_btnChatSend;
	
};

//////////////////////////////////////////////////////////////////////////////
//
// CreateMissionDlg
//
//////////////////////////////////////////////////////////////////////////////


class CreateMissionDlg : public TrekDialog,  public EventTargetContainer<CreateMissionDlg>, 
    public IMenuCommandSink
{
public:
    CreateMissionDlg(Window* pwndParent);
    ~CreateMissionDlg(void);

	// view notifications
    virtual void OnAddPlayer(MissionInfo* pMissionInfo, SideID sideID, PlayerInfo* pPlayerInfo);

    bool OnPickCiv(int civID);

    bool OnSelMapType();

    void OnMenuCommand(IMenuItem* pitem);
        
protected:

    virtual bool OnCommand(WPARAM wParam, LPARAM lParam);

    // message handling
    virtual BOOL OnInitDialog();


private:

    ZString             GetMapTypeName(int nMapType);

    TRef<ButtonPane> CreateCheckButtonPane(const WinPoint& ptButton, bool bOn);

  
    TRef<ButtonPane>    m_pbuttonPaneEjectPods;
    TRef<ButtonPane>    m_pbuttonPaneAllowPrivateTeams;
    TRef<ButtonPane>    m_pbuttonPanePowerups;
    TRef<ButtonPane>    m_pbuttonPaneAllowJoiners;
    TRef<ButtonPane>    m_pbuttonPaneStations;
    TRef<ButtonPane>    m_pbuttonPaneScoresCount;
    TRef<ButtonPane>    m_btnMapType;
    TRef<IMenu>         m_menuMapType;
    int                 m_nMapType;

    TrekEdit            m_editNumTeams;
    TrekEdit            m_editMaxPlayers;
    TrekEdit            m_editMinPlayers;
    TrekEdit            m_editMapType;

    TVector<TRef<PaneWindow> >  m_rgppaneWindows;
   
    TrekImgButton       m_btnBack;
    TrekImgButton       m_btnCreate;
};