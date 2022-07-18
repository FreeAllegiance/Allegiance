#ifndef _enginewindow_h_
#define _enginewindow_h_

#include <window.h>
#include <ztime.h>

#include "caption.h"
#include "engine.h"
#include "inputengine.h"
#include "menu.h"
#include "VideoSettingsDX9.h"

class Context;
class EngineApp;
class GroupImage;
class Surface;
class TransformImage;
class TranslateTransform2;
class WrapImage;

class ExponentialMovingAverageTracker {
private:
    double m_avg;
    double m_alpha;

public:
    ExponentialMovingAverageTracker(int approx_window_size) :
        m_avg(0),
        m_alpha( 1.0f / approx_window_size)
    {}

    void AddSample(double value) {
        m_avg = (value * m_alpha) + (1.0 - m_alpha) * m_avg;
    }

    double getAverage() const {
        return m_avg;
    }
};

class MaximumTracker {
private:
    double m_max;
    int m_count;
    int m_samples_ago;

public:
    MaximumTracker(int size) :
        m_count(size),
        m_max(0.0),
        m_samples_ago(size)
    {
    }

    void AddSample(double value) {
        m_samples_ago++;
        if (m_samples_ago > m_count || m_max <= value) {
            m_max = value;
            m_samples_ago = 0;
        }
    }

    double getMaximum() const {
        return m_max;
    }
};

class MultiTracker {
public:
    ExponentialMovingAverageTracker avg;
    MaximumTracker max;

    MultiTracker(int window_size) :
        avg(window_size),
        max(window_size)
    {}
};

class RenderTimings {
private:
    std::map<int, MultiTracker> m_map;

public:
    RenderTimings() :
        m_map({
            { 10, MultiTracker(10) },
            { 100, MultiTracker(100) },
            { 1000, MultiTracker(1000) },
            { 10000, MultiTracker(10000) }
            })
    {

    }

    void AddSample(double value) {
        for (auto& kv : m_map) {
            kv.second.avg.AddSample(value);
            kv.second.max.AddSample(value);
        }
    }

    const std::map<int, MultiTracker>& getMap() const {
        return m_map;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// A window with an associated engine object
//
//////////////////////////////////////////////////////////////////////////////

class EngineWindow :
    public Window,
    public IKeyboardInput,
    public ICaptionSite,
    public ButtonEvent::Sink
{
protected:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Types
    //
    //////////////////////////////////////////////////////////////////////////////

    class ModeData {
    public:
        WinPoint m_size;
        bool     m_bStretch;

        ModeData(const WinPoint& size, bool bStretch) :
            m_size(size),
            m_bStretch(bStretch)
        {
        }

        ModeData(const ModeData& data) :
            m_size(data.m_size),
            m_bStretch(data.m_bStretch)
        {
        }
    };

    //////////////////////////////////////////////////////////////////////////////
    //
    // Static Members
    //
    //////////////////////////////////////////////////////////////////////////////

    static ModeData s_pmodes[];
    static int      s_countModes;
    static int      s_forceHitTestCount;
    static bool     s_cursorIsHidden;

    //////////////////////////////////////////////////////////////////////////////
    //
    // Members
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<EngineConfigurationWrapper> m_pConfiguration;
    TRef<ValueList> m_pConfigurationUpdater;

    std::map<std::string, RenderTimings> m_timings = {};
    Time m_tPreviousFramePresented;
    uint32_t m_iPreviousFrameTRefAdded;
    uint32_t m_iPreviousFrameTRefRemoved;

    TRef<Engine>               m_pengine;
    TRef<InputEngine>          m_pinputEngine;
    TRef<ButtonEvent::Sink>    m_pbuttonEventSink;
    TRef<ModifiablePointValue> m_ppointMouse;

    TRef<Surface>              m_psurface;
    TRef<ICaption>             m_pcaption;

    TRef<IKeyboardInput>       m_pkeyboardInput;

    TRef<GroupImage>           m_pgroupImage;
    TRef<WrapImage>            m_pwrapImage;
    TRef<TransformImage>       m_ptransformImageCursor;
    TRef<TranslateTransform2>  m_ptranslateTransform;
    TRef<WrapImage>            m_pimageCursor;

    WinPoint                   m_offsetWindowed;

    bool                       m_bMovingWindow;
    bool                       m_bSizeable;
    bool                       m_bMinimized;
    bool                       m_bHideCursor;
    bool                       m_bCaptured;
    bool                       m_bHit;
    bool                       m_bInvalid;
    bool                       m_bActive;
    bool                       m_bShowCursor;
    bool                       m_bRestore;
    bool                       m_bMouseInside;
    bool                       m_bMoveOnHide;
	TRef<SimpleModifiableValue<bool>> m_pPreferredFullscreen;
	bool						m_bWindowStateMinimised;
	bool						m_bWindowStateRestored;
	bool						m_bClickBreak;
    bool m_bRenderingEnabled;

    int                        m_modeIndex;

    TRef<ModifiableRectValue>  m_prectValueScreen;
    TRef<ModifiableRectValue>  m_prectValueRender;
    TRef<WrapRectValue>        m_pwrapRectValueRender;

    TRef<ModifiableNumber>     m_pnumberTime;
    Time                       m_timeStart;
    Time                       m_timeLast;
    Time                       m_timeLastFrame;
    Time                       m_timeCurrent;
    Time                       m_timeLastMouseMove;
    Time                       m_timeLastClick;

    //
    // Input
    //

    TRef<ButtonEvent::Sink>    m_peventSink;

    TRef<EventSourceImpl> m_pcloseEventSource;
    TRef<TEvent<Time>::SourceImpl> m_pevaluateFrameEventSource;
    TRef<TEvent<bool>::SourceImpl> m_pactivateEventSource;

    //
    // Performance
    //

    char                       m_pszLabel[20];
    bool                       m_bFPS;
    int                        m_indexFPS;
    float                      m_frameCount;
    int                        m_frameTotal;
    ZString                    m_strPerformance1;
    ZString                    m_strPerformance2;
    TRef<IEngineFont>          m_pfontFPS;

    double                     m_triangles;
    double                     m_tpf;
    double                     m_ppf;  
    double                     m_tps;  
    double                     m_fps;  
    double                     m_mspf; 

    //////////////////////////////////////////////////////////////////////////////
    //
    // Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    //
    // Performance
    //

    void UpdatePerformanceCounters(Context* pcontext, Time timeCurrent);
    void RenderPerformanceCounters(Surface* psurface);

    //
    // Implementation methods
    //

    void UpdateRectValues();
    void UpdateWindowStyle();
    void UpdateSurfacePointer();
    void UpdateCursor();

    bool CheckDeviceAndUpdate();
    
    void UpdateInput();
    void HandleMouseMessage(UINT message, const Point& point, UINT nFlags = 0);

    void DoIdle();
    bool ShouldDrawFrame();
    bool RenderFrame();
    void UpdateFrame();
    void Invalidate();

    //
    // menu
    //

    ZString GetRendererString();
    ZString GetDeviceString();
    ZString GetResolutionString();
    ZString GetRenderingString();
    ZString GetPixelFormatString(); // KGJV 32B

public:
    EngineWindow(
        EngineConfigurationWrapper* pConfiguration,
        const ZString&     strTitle         = ZString(),
        const WinRect&     rect             = WinRect(0, 0, -1, -1),
        const WinPoint&    sizeMin          = WinPoint(1, 1),
              HMENU        hmenu            = NULL
    );

    ~EngineWindow();

    //
    // Static Functions
    //

    static void DoHitTest();

    //
    // EngineWindow methods
    //

    // These need to be set here before this object is fully functional
    void SetEngine(Engine* pengine);

    Number*          GetTime()           { return m_pnumberTime;             }
    Time             GetTimeStart()      { return m_timeStart;               }
    Engine*          GetEngine()         { return m_pengine;                 }
    bool             GetFullscreen()     { return m_pengine->IsFullscreen(); }
    bool             GetShowFPS()        { return m_bFPS;                    }
    InputEngine*     GetInputEngine()    { return m_pinputEngine;            }
    const Point&     GetMousePosition()  { return m_ppointMouse->GetValue(); }
    ModifiablePointValue* GetMousePositionModifiable() { return m_ppointMouse; }
	Time&		   	 GetMouseActivity()  { return m_timeLastMouseMove;		 } //Imago: Added to adjust AFK status from mouse movment
    bool             GetActive()         { return m_bActive;                 }
    const TRef<IKeyboardInput>& GetKeyboardInput() { return m_pkeyboardInput; };

    void SetRenderingEnabled(bool bEnabled) {
        m_bRenderingEnabled = bEnabled;
    }

    IEventSource* GetOnCloseEventSource() {
        return m_pcloseEventSource;
    }

    TEvent<Time>::Source* GetEvaluateFrameEventSource() {
        return m_pevaluateFrameEventSource;
    }

    TEvent<bool>::Source* GetActivateEventSource() {
        return m_pactivateEventSource;
    }

    RectValue* GetScreenRectValue();
    RectValue* GetRenderRectValue();

    void SetFullscreen(bool bFullscreen);
    void SetSizeable(bool bSizeable);
    void SetFullscreenSize(const Vector& point);
    void ChangeFullscreenSize(bool bLarger);

    WinPoint GetSize();
    WinPoint GetWindowedSize();
    WinPoint GetFullscreenSize();

    TRef<PointValue> GetResolution();

    void OutputPerformanceCounters();
    void SetImage(Image* pimage);
    void SetCursorImage(Image* pimage);
    Image* GetCursorImage(void) const;
    void SetCaption(ICaption* pcaption);
    void SetHideCursorTimer(bool bHideCursor);

    void SetShowFPS(bool bFPS, const char* pszLabel = NULL);
    void ToggleShowFPS();
    void SetMoveOnHide(bool bMoveOnHide);

    //
    // App exit
    //

    virtual void StartClose();

    //
    // subclass overrides
    //

    virtual ZString GetFPSString(float dtime, float mspf, Context* pcontext);

    virtual void RenderSizeChanged(bool bSmaller) {
        int x = (int)m_pengine->GetResolutionSizeModifiable()->GetValue().X();
        int y = (int)m_pengine->GetResolutionSizeModifiable()->GetValue().Y();
        m_pConfiguration->GetGraphicsResolutionX()->SetValue((float)x);
        m_pConfiguration->GetGraphicsResolutionY()->SetValue((float)y);
    }

    //
    // ICaptionSite methods
    //

    void OnCaptionMinimize();
    void OnCaptionMaximize();
    void OnCaptionFullscreen();
    void OnCaptionRestore();
    void OnCaptionClose();

    //
    // ButtonEvent::Sink methods
    //

    bool OnEvent(ButtonEvent::Source* pevent, ButtonEventData data);

    //
    // IInputProvider methods
    //

    void SetCursorPos(const Point& point);
    bool IsDoubleClick();
    void ShowCursor(bool bShow);

    //
    // Window methods
    //

    void RectChanged();
    void OnClose();
    bool OnActivate(UINT nState, bool bMinimized);
    bool OnActivateApp(bool bActive);
    bool OnMouseMessage(UINT message, UINT nFlags, const WinPoint& point);
    bool OnSysCommand(UINT uCmdType, const WinPoint &point);
    void OnPaint(HDC hdc, const WinRect& rect);
    bool OnWindowPosChanging(WINDOWPOS* pwp);

    //
    // IObject methods
    //

    bool IsValid();
};

#endif
