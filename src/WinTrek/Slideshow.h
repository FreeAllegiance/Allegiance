#pragma once

#include "screen.h"

//////////////////////////////////////////////////////////////////////////////
//
// Slideshow Screen
//
//////////////////////////////////////////////////////////////////////////////

class Slideshow :
    public Screen,
    public IEventSink,
    public IKeyboardInput
{
    private:
        TRef<IObjectList>       m_pSlideList;
        TRef<Image>             m_pImage;
        TRef<WrapImage>         m_pWrapImage;
        bool                    m_bNextSlide;
        TRef<ISoundInstance>    m_pSoundInstance;
        TRef<IKeyboardInput>    m_pkeyboardInputOldFocus;
        TRef<IEventSink>        m_pEventSink;
        bool                    m_bInTimer;

        class PickImage : public Image
        {
            private:
                Slideshow*  m_pSlideshow;

            public:
                PickImage (Slideshow* pSlideshow) :
                    m_pSlideshow(pSlideshow)
                {
                }

                MouseResult HitTest (IInputProvider* pProvider, const Point& point)
                {
                    return MouseResultHit();
                }

                MouseResult Button (IInputProvider* pProvider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown)
                {
                    if (bDown)
                        m_pSlideshow->Picked();
                    return MouseResult();
                }
        };

                void        CleanUpTimers (void);
                void        StopSound (void);

        static  bool        m_bIsInSlideShow;

    public:
                /* void */  Slideshow (Modeler* pmodeler, const ZString& strNamespace);
                /* void */  ~Slideshow (void);
                void        NextSlide (void);
        virtual Image*      GetImage (void);
        virtual void        OnFrame (void);
        virtual void        Dismiss (void);
        virtual bool        OnEvent (IEventSource* pevent);
        virtual void        Picked (void);
        virtual bool        OnKey (IInputProvider* pprovider, const KeyState& ks, bool& fForceTranslate);
        static  bool        IsInSlideShow (void);
};