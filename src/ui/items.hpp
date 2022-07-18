#pragma once

#include "enginep.h"
#include "font.h"
#include "valuetransform.h"
#include "imagetransform.h"
#include "event.h"
#include "image.h"
#include "Exposer.h"

typedef TEvent<Point> PointEvent;

class MouseEventImage : public WrapImage {
    std::map<std::string, TRef<EventSourceImpl>> m_mapEventSources;
    std::map<std::string, TRef<PointEvent::SourceImpl>> m_mapPointEventSources;
    bool m_bDown = false;
    Point m_pointDown;

    bool m_bCaptured = false;
    bool m_bChildCaptured = false;

public:

    MouseEventImage(Image* wrapped) :
        WrapImage(wrapped)
    {
    }

    TRef<IEventSource> GetEventSource(std::string string) {
        auto found = m_mapEventSources.find(string);
        if (found != m_mapEventSources.end()) {
            return found->second;
        }

        TRef<EventSourceImpl> pEventSource = new EventSourceImpl();
        m_mapEventSources[string] = pEventSource;
        return pEventSource;
    }

    TRef<PointEvent::Source> GetPointEventSource(std::string string) {
        auto found = m_mapPointEventSources.find(string);
        if (found != m_mapPointEventSources.end()) {
            return found->second;
        }

        TRef<PointEvent::SourceImpl> pEventSource = new PointEvent::SourceImpl();
        m_mapPointEventSources[string] = pEventSource;
        return pEventSource;
    }

    void Trigger(std::string string) {
        auto found = m_mapEventSources.find(string);
        if (found != m_mapEventSources.end()) {
            found->second->Trigger();
        }
    }

    void TriggerPoint(std::string string, Point& point) {
        auto found = m_mapPointEventSources.find(string);
        if (found != m_mapPointEventSources.end()) {
            found->second->Trigger(point);
        }
    }

    void MouseEnter(IInputProvider* pprovider, const Point& point) {
        Trigger("mouse.enter");
    }

    void MouseLeave(IInputProvider* pprovider) {
        //m_bDown = false; //reset state
        Trigger("mouse.leave");
    }

    void TriggerMouseButton(std::string button_name, std::string what) {
        Trigger("mouse." + button_name + "." + what);
    }

    void MouseMove(IInputProvider* pprovider, const Point& point, bool bCaptured, bool bInside) override
    {
        if (bCaptured) {
            TriggerPoint("drag", point - m_pointDown);
            m_pointDown = point;
        }
    }

    MouseResult HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured)
    {
        return GetImage()->HitTest(pprovider, point, m_bChildCaptured);
    }

    MouseResult Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown)
    {
        MouseResult result;
        // inspired by button.cpp~:~690
        if (button == 0 || button == 1) {
            std::string button_name = button == 0 ? "left" : "right";

            if (bDown) {
                m_bDown = true;
                m_pointDown = point;
                TriggerMouseButton(button_name, "down");

                if (pprovider->IsDoubleClick()) {
                    TriggerMouseButton(button_name, "doubleclick");
                }
            }
            else {
                bool wasDown = m_bDown;
                m_bDown = false;
                TriggerMouseButton(button_name, "up");

                if (wasDown) {
                    TriggerMouseButton(button_name, "click");
                }
            }

            if (button == 0) {
                if (bDown) {
                    result = MouseResultCapture();
                } else if (bCaptured) {
                    TriggerPoint("drag", point - m_pointDown);
                    result = MouseResultRelease();
                }
            }
        }
        else if (button == 8 || button == 9) {
            std::string scroll_name = button == 8 ? "down" : "up";

            Trigger("scroll." + scroll_name);
        }

        MouseResult child_result = WrapImage::Button(pprovider, point, button, bCaptured, bInside, bDown);

        if (child_result.Test(MouseResultRelease())) {
            m_bChildCaptured = false;
        }
        else if (child_result.Test(MouseResultCapture())) {
            m_bChildCaptured = true;
        }

        if (result.Test(MouseResultRelease())) {
            m_bCaptured = false;
        }
        else if (result.Test(MouseResultCapture())) {
            m_bCaptured = true;
        }

        if ((m_bChildCaptured || m_bCaptured) && bCaptured == false) {
            //we want to capture, but we aren't captured yet
            return MouseResultCapture();
        }

        if (m_bChildCaptured == false && m_bCaptured == false && bCaptured) {
            //we don't want to capture, but we are captured
            return MouseResultRelease();
        }

        return MouseResult();
    }
};

template<typename A>
TRef<TStaticValue<A>> wrapValue(sol::object a) {
    TRef<TStaticValue<A>> converted_a;
    if (a.is<A>()) {
        converted_a = new TStaticValue<A>(a.as<A>());
    }
    else if (a.is<TRef<TStaticValue<A>>>()) {
        return a.as<TRef<TStaticValue<A>>>();
    }
    else if (a.is<TStaticValue<A>*>()) {
        converted_a = a.as<TStaticValue<A>*>();
    }
    else {
        // force a cast, this sometimes still works, exception otherwise
        converted_a = new TStaticValue<A>(a.as<A>());
    }
    return converted_a;
};

template<>
TRef<StringValue> wrapValue<ZString>(sol::object a) {
    if (a.is<std::string>()) {
        return (TRef<StringValue>)new StringValue(ZString(a.as<std::string>().c_str()));
    }

    typedef ZString A;
    TRef<TStaticValue<A>> converted_a;
    if (a.is<A>()) {
        converted_a = new TStaticValue<A>(a.as<A>());
    }
    else if (a.is<TRef<TStaticValue<A>>>()) {
        return a.as<TRef<TStaticValue<A>>>();
    }
    else if (a.is<TStaticValue<A>*>()) {
        converted_a = a.as<TStaticValue<A>*>();
    }
    else {
        // force a cast, this sometimes still works, exception otherwise
        converted_a = new TStaticValue<A>(a.as<A>());
    }
    return converted_a;
};

TRef<StringValue> wrapString(sol::object a) {
    return wrapValue<ZString>(a);
};