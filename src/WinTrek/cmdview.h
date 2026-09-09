#ifndef	TREKGEO_H
#define	TREKGEO_H

/////////////////////////////////////////////////////////////////////////////
//
// ImageStringPane 
//
/////////////////////////////////////////////////////////////////////////////

class ImageStringPane : public Pane
{
private:
    TRef<StringPane> m_pStringPane;
    
public:
    enum {
        ImageLeft = 0,
        ImageRight = 1
    };

    ImageStringPane(LPCSTR szText, LPCSTR szImage, bool bUp = true, int Layout = ImageLeft);
    virtual void UpdateLayout();
    virtual void SetString(LPCSTR szText);

};

/////////////////////////////////////////////////////////////////////////////
//
// ImageStringButtonPane 
//
/////////////////////////////////////////////////////////////////////////////

class ImageStringButtonPane : public Pane
{
private:
    TRef<ImageStringPane> m_pImageStringPaneUp;
    TRef<ImageStringPane> m_pImageStringPaneDn;
    TRef<ButtonPane> m_pButtonPane;

public:

    ImageStringButtonPane(LPCSTR szText, LPCSTR szImageUp, LPCSTR szImageDn, int Layout = ImageStringPane::ImageLeft);
    virtual void UpdateLayout();
    virtual IEventSource* GetEventSource();
    virtual void SetString(LPCSTR szText);

};


//////////////////////////////////////////////////////////////////////////////
//
// CommandGeo
//
//////////////////////////////////////////////////////////////////////////////

#define CmdViewMaxIconScreenSize  7.0f

class CommandGeo : public Geo
{
private:
    TVector<VertexL>  m_vertices;
    TVector<WORD>     m_indices;
    TRef<IclusterIGC> m_pcluster;
    float             m_zGrid;
    float             m_radius;

    
    //TRef<Surface>  m_top; //GetModeler()->LoadImage("toparrowbmp", true)->GetSurface();

    void DrawShips(Context* pcontext);
    void DrawSelectedPaths(Context* pcontext);
    void DrawTop(Context* pcontext);

public:
    CommandGeo(float radius, float zGrid, int nSegments); //Surface* top
    void Render(Context* pcontext);
    void SetCluster(IclusterIGC* pcluster);

    //
    // Value members
    //

    ZString GetFunctionName() { return "CommandGeo"; }

    static const Color s_colorGrid;
    static const Color s_colorGridMajor;
    static const Color s_colorDropLineUp;
    static const Color s_colorDropLineDown;
    static const Color s_colorFeet;
    static const Color s_colorNeutral;
};


#endif
