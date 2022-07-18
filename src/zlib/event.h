#ifndef _message_H_
#define _message_H_

#include <functional>

#include "tlist.h"
#include "tref.h"
#include "zstring.h"

/////////////////////////////////////////////////////////////////////////////
//
// Event
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// Template Pair
//
/////////////////////////////////////////////////////////////////////////////

template<class FirstType, class SecondType>
class TPair {
private:
    FirstType  m_first;
    SecondType m_second;

public:
    TPair(const FirstType& first, const SecondType& second) :
        m_first(first),
        m_second(second)
    {
    }

    const FirstType&  GetFirst()  const { return m_first;  }
    const SecondType& GetSecond() const { return m_second; }
};

/////////////////////////////////////////////////////////////////////////////
//
// Template Events
//
/////////////////////////////////////////////////////////////////////////////

template<class ...Types>
class TEvent {
public:
    class Sink;
    class Source;
    class SourceImpl;

    class Cleanable {
    private:
        TRef<Source> m_psource;
        TRef<Sink> m_psink;

    public:
        Cleanable(const Cleanable&& a):
            m_psource(a.m_psource),
            m_psink(a.m_psink)
        {
        };

        Cleanable() = default;
        Cleanable(const Cleanable&) = delete;
        Cleanable& operator=(const Cleanable&) = delete;

        Cleanable& operator=(Cleanable&& a) {
            m_psource = a.m_psource;
            m_psink = a.m_psink;
            a.m_psource = nullptr;
            a.m_psink = nullptr;
            return *this;
        };

        Cleanable(const TRef<Source>& psource, const TRef<Sink>& psink) :
            m_psource(psource),
            m_psink(psink)
        {
            if (m_psink) {
                m_psource->AddSink(psink);
            }
        }

        ~Cleanable() {
            Cleanup();
        }

        void Cleanup() {
            if (m_psink) {
                m_psource->RemoveSink(m_psink);
                m_psink = nullptr;
                m_psource = nullptr;
            }
        }
    };

    class Source : public IObject {
    public:
        static Source* GetNever();

        static TRef<SourceImpl> Create()
        {
            return new SourceImpl();
        }

        virtual void    AddSink(Sink* psink) = 0;
        virtual void RemoveSink(Sink* psink) = 0;
        virtual Cleanable AddSinkManaged(Sink* psink) {
            TRef<Sink> psinkDelegate = Sink::CreateDelegate(psink);
            return Cleanable(TRef<Source>(this), psinkDelegate);
        };
    };

    class Sink : public IObject {
    public:
        static TRef<Sink> CreateDelegate(Sink* psink)
        {
            return new Delegate(psink);
        }

        virtual bool OnEvent(Source* pevent, Types... values) = 0;
    };

    class SourceImpl : public Source {
    private:
        TList<TRef<Sink> > m_listSinks;

    public:
        void AddSink(Sink* psink)
        {
            m_listSinks.PushFront(psink);
        }

        void RemoveSink(Sink* psink)
        {
            m_listSinks.Remove(psink);
        }

        void Trigger(Types... values)
        {
            typename TList<TRef<Sink> >::Iterator iter(m_listSinks);

            while (!iter.End()) {
                if (!iter.Value()->OnEvent(this, values...)) {
                    iter.Remove();
                } else {
                    iter.Next();
                }
            }
        }
    };

// KGJV // private:
    class Delegate : public Sink {
    private:
        Sink* m_psink;

    public:
        Delegate(Sink* psink) :
            m_psink(psink)
        {
        }

        bool OnEvent(Source* pevent, Types... values)
        {
            return m_psink->OnEvent(pevent, values...);
        }
    };
};

typedef TEvent<>::Source IEventSource;
typedef TEvent<>::Sink IEventSink;

//class IEventSource : public TEvent<>::Source {};

/////////////////////////////////////////////////////////////////////////////
//
// StringEvents
//
/////////////////////////////////////////////////////////////////////////////

typedef TEvent<ZString>         StringEvent;
typedef StringEvent::Source     IStringEventSource;
typedef StringEvent::Sink       IStringEventSink;
typedef StringEvent::SourceImpl StringEventSourceImpl;

/////////////////////////////////////////////////////////////////////////////
//
// Event Implementation
//
/////////////////////////////////////////////////////////////////////////////

class EventSourceImpl : public TEvent<>::Source {
private:
    TList<TRef<IEventSink> > m_listSinks;

public:
    void AddSink(IEventSink* psink);
    void RemoveSink(IEventSink* psink);
    void Trigger();
};

/////////////////////////////////////////////////////////////////////////////
//
// Integer Event
//
/////////////////////////////////////////////////////////////////////////////

/*
typedef TEvent<int>::Source     IIntegerEventSource;
typedef TEvent<int>::Sink       IIntegerEventSink;
typedef TEvent<int>::SourceImpl IntegerEventSourceImpl;
*/

class IIntegerEventSink;
class IIntegerEventSource : public TEvent<>::Source {
public:
    virtual void AddSink(IIntegerEventSink* psink)    = 0;
    virtual void RemoveSink(IIntegerEventSink* psink) = 0;
};

class IIntegerEventSink : public IObject {
public:
    static TRef<IIntegerEventSink> CreateDelegate(IIntegerEventSink* psink);

    virtual bool OnEvent(IIntegerEventSource* pevent, int value) = 0;
};

/////////////////////////////////////////////////////////////////////////////
//
// Integer Event Implementation
//
/////////////////////////////////////////////////////////////////////////////

class IntegerEventSourceImpl : public IIntegerEventSource {
private:
    TList<TRef<IEventSink> >        m_listSinks;
    TList<TRef<IIntegerEventSink> > m_listIntegerSinks;

public:
    void AddSink(IEventSink* psink);
    void RemoveSink(IEventSink* psink);

    void AddSink(IIntegerEventSink* psink);
    void RemoveSink(IIntegerEventSink* psink);
    void Trigger(int value);
};

/////////////////////////////////////////////////////////////////////////////
//
// Timer Event
//
/////////////////////////////////////////////////////////////////////////////

class ITimerEventSource : public TEvent<>::Source {
public:
    virtual void AddSink(IEventSink* psink, float when) = 0;
    virtual void RemoveSink(IEventSink* psink)          = 0;
};

/////////////////////////////////////////////////////////////////////////////
//
// EventTargetContainer
//
/////////////////////////////////////////////////////////////////////////////

class IAllegEventTarget : public IObject
{
public:
    virtual void Disconnect() = 0;
};

template<class ObjectClass>
class TIntegerEventTarget : public IIntegerEventSink, public IAllegEventTarget
{
private:

    typedef bool (ObjectClass::*PFNOnEventMember)(int value);

    ObjectClass* m_pobject;
    PFNOnEventMember m_pfn;
    TRef<IIntegerEventSource> m_psource;

public:
    TIntegerEventTarget(ObjectClass* pobject, 
            PFNOnEventMember pfn,
            IIntegerEventSource* psource) :
        m_pobject(pobject),
        m_pfn(pfn),
        m_psource(psource)
    {
        m_psource->AddSink(this);
    }

    void Disconnect()
    {
        m_psource->RemoveSink(this);
    }

    bool OnEvent(IIntegerEventSource* pevent, int value)
    {
        return (m_pobject->*m_pfn)(value);
    }
};

template<class ObjectClass>
class TEventTarget : public IEventSink, public IAllegEventTarget
{
private:

    typedef bool (ObjectClass::*PFNOnEventVoidMember)(void);

    ObjectClass* m_pobject;
    PFNOnEventVoidMember m_pfn;
    TRef<IEventSource> m_psource;

public:
    TEventTarget(ObjectClass* pobject, 
            PFNOnEventVoidMember pfn,
            IEventSource* psource) :
        m_pobject(pobject),
        m_pfn(pfn),
        m_psource(psource)
    {
        m_psource->AddSink(this);
    }

    TEventTarget(ObjectClass* pobject, 
            PFNOnEventVoidMember pfn,
            ITimerEventSource* psource,
            float when) :
        m_pobject(pobject),
        m_pfn(pfn),
        m_psource(psource)
    {
        psource->AddSink(this, when);
    }

    void Disconnect()
    {
        m_psource->RemoveSink(this);
    }

    bool OnEvent(IEventSource* pevent)
    {
        return (m_pobject->*m_pfn)();
    }
};


template<class DerivedClass>
class EventTargetContainer : public IObject 
{
private:
    typedef bool (DerivedClass::*PFNOnEventMember)(int value);
    typedef TIntegerEventTarget<DerivedClass> IntegerEventTarget;

    typedef bool (DerivedClass::*PFNOnEventVoidMember)(void);
    typedef TEventTarget<DerivedClass> EventTarget;
    
    TList<TRef<IAllegEventTarget> > m_lstTargets;

public:
    virtual ~EventTargetContainer()
    {
        TList<TRef<IAllegEventTarget> >::Iterator iter(m_lstTargets);

        while (!iter.End()) {
            iter.Value()->Disconnect();
            iter.Next();
        }
    }

    void AddEventTarget(PFNOnEventMember pfn, IIntegerEventSource* psource)
    {
        m_lstTargets.PushEnd(new IntegerEventTarget((DerivedClass*)this, pfn, psource));
    }

    void AddEventTarget(PFNOnEventVoidMember pfn, IEventSource* psource)
    {
        m_lstTargets.PushEnd(new EventTarget((DerivedClass*)this, pfn, psource));
    }

    void AddEventTarget(PFNOnEventVoidMember pfn, ITimerEventSource* psource, float when)
    {
        m_lstTargets.PushEnd(new EventTarget((DerivedClass*)this, pfn, psource, when));
    }
};

class CallbackSink : public IEventSink {
private:
    std::function<void()> m_funcCallback;

public:
    CallbackSink(std::function<void()> funcCallback) :
        m_funcCallback(funcCallback)
    {}

    bool OnEvent(IEventSource* pevent) {
        m_funcCallback();
        return true;
    }
};

template <class ...Types>
class CallbackValueSink : public TEvent<Types...>::Sink {
private:
    std::function<bool(Types...)> m_funcCallback;

public:
    CallbackValueSink(std::function<bool(Types...)> funcCallback) :
        m_funcCallback(funcCallback)
    {}

    bool OnEvent(typename TEvent<Types...>::Source* pevent, Types ... values) override {
        return m_funcCallback(values...);
    }
};

#endif
