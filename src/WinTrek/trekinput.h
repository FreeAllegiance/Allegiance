#pragma once

//////////////////////////////////////////////////////////////////////////////
//
// Trek Input
//
//////////////////////////////////////////////////////////////////////////////

class TrekInputSite : public IObject {
public:
    virtual void OnTrekKey(TrekKey tk) = 0;
};

class TrekInput : public IObject {
public:
    virtual bool         LoadMap(const ZString& str)                                               = 0;
	virtual void		 ClearButtonStates()													   = 0; // yp - Your_Persona allow for clearButtonStates.(buttons get stuck patch) aug-03-2006
    virtual void         SetFocus(bool bFocus)                                                     = 0;
    virtual void         PlayFFEffect(ForceEffectID effectID, LONG lDirection = 0)                 = 0;
    virtual float        GetAxis(int index)                                                        = 0;
    virtual bool         IsAxisValid(int index)                                                    = 0;
    virtual bool         IsTrekKeyDown(TrekKey trekkey, bool bCheckKeyboard)                       = 0;
    virtual TrekKey      TranslateKeyMessage(const KeyState& ks)                                   = 0;
    virtual TrekKey      HandleKeyMessage(const KeyState& ks)                                      = 0;
    virtual void         GetButtonTrekKeys(TrekInputSite* psite)                                   = 0;
    virtual ZString      GetFPSString()                                                            = 0;
    virtual TRef<IPopup> CreateInputMapPopup(Modeler* pmodeler, IEngineFont* pfont, Number* ptime) = 0;
   
    // Imago 8/14/09 expose trek input site
    virtual TRef<TrekInputSite> GetInputSite()                                                     = 0;
    virtual void                SetInputSite(TrekInputSite* psite)                                 = 0;
    virtual void                SetMouseTrekKey(TrekKey tk, Boolean* pboolDown)					   = 0;

    // Imago 8/14/09 mouse mappings for use outside virtual joystick
    virtual TrekKey      OnWheelClick()                                                            = 0;
    virtual TrekKey      OnWheelDown()                                                             = 0;
    virtual TrekKey      OnWheelUp()                                                               = 0;
    virtual TrekKey      OnXButton1()                                                              = 0;
    virtual TrekKey      OnXButton2()                                                              = 0;
    virtual TrekKey      OnXButton3()                                                              = 0;
    virtual TrekKey      OnXButton4()                                                              = 0;
    virtual TrekKey      OnXButton5()                                                              = 0;
};

TRef<TrekInput> CreateTrekInput(HINSTANCE hInst, HWND hwnd, InputEngine* pinputEngine, JoystickImage* pjoystickImage);

#define INPUTMAP_FILE         "inputmap1"
#define DEFAULTINPUTMAP_FILE  "defaultinputmap"