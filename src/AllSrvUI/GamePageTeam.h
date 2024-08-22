#pragma once

/////////////////////////////////////////////////////////////////////////////
// GamePageTeam.h : header file
//

#include "ComboValueTranslator.h"


/////////////////////////////////////////////////////////////////////////////
// Forward Declaration

class CGameParamsSheet;


/////////////////////////////////////////////////////////////////////////////
// CGamePageTeam dialog

class CGamePageTeam : public CPropertyPage
{
// Declarations
public:
  DECLARE_DYNAMIC(CGamePageTeam)
  DECLARE_MESSAGE_MAP()

// Construction
public:
  CGamePageTeam();

// Dialog Data
  //{{AFX_DATA(CGamePageTeam)
  enum { IDD = IDD_TEAM_DYNAMICS };
  int   m_eTeamCount;
  int   m_eMaxTeamInbalance;
  int   m_eMinTeamPlayers;
  int   m_eMaxTeamPlayers;
  int   m_eSkillLevels;
  int   m_eTeamLives;
  int   m_eStartingMoney;
  int   m_eTotalMoney;
  //}}AFX_DATA

// Attributes
public:

// Overrides
public:
  // ClassWizard generate virtual function overrides
  //{{AFX_VIRTUAL(CGamePageTeam)
  public:
  virtual void OnOK();
  virtual BOOL OnSetActive();
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:
#if 0
  BEGIN_ComboTranslator(TeamCount, 1)
    2, 3, 4, 5, 6
  END_ComboTranslator(TeamCount)

  BEGIN_ComboTranslator(MaxTeamInbalance, 2)
    1, 2, -1
  END_ComboTranslator(MaxTeamInbalance)

  BEGIN_ComboTranslator(MinTeamPlayers, 0)
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 30, 40, 50
  END_ComboTranslator(MinTeamPlayers)

  BEGIN_ComboTranslator(MaxTeamPlayers, 10)
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 30, 40, 50
  END_ComboTranslator(MaxTeamPlayers)

  BEGIN_ComboTranslator(SkillLevels, 5)
    1, 2, -1
  END_ComboTranslator(SkillLevels)
#endif

// Message Handlers
protected:
  // Generated message map functions
  //{{AFX_MSG(CGamePageTeam)
  virtual BOOL OnInitDialog();
  //}}AFX_MSG

// Data Members
protected:
};


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
/////////////////////////////////////////////////////////////////////////////