#pragma once

#include <functional>
#include <list>
#include <memory>

#include "model.h"
#include "event.h"
#include "valuetransform.h"

#include "Exposer.h"

class UiObjectContainer : public IObject {

private:
    std::map<std::string, std::shared_ptr<Exposer>> m_map;

public:
    UiObjectContainer(std::map<std::string, std::shared_ptr<Exposer>> map) :
        m_map(map)
    {
    }

    template <typename Type>
    const Type& Get(std::string key) const {
        auto found = m_map.find(key);
        if (found == m_map.end()) {
            throw std::runtime_error("Key not found: " + key);
        }

        return (const Type&)(*found->second);
    }

    std::shared_ptr<Exposer> GetExposer(std::string key) const {
        auto found = m_map.find(key);
        if (found == m_map.end()) {
            throw std::runtime_error("Key not found: " + key);
        }
        return found->second;
    }

    TRef<StringValue> GetString(std::string key) const {
        return Get<TRef<StringValue>>(key);
    }

    TRef<Boolean> GetBoolean(std::string key) const {
        return Get<TRef<Boolean>>(key);
    }

    TRef<Number> GetNumber(std::string key) const {
        return Get<TRef<Number>>(key);
    }

};


// Maybe remove when we really don't care about mdl logging anymore
class ModifiableUiStateName {
public:
    static ZString GetName() { return "ModifiableState"; }
};

class UiState : public UiObjectContainer {
public:
    typedef std::map<std::string, std::shared_ptr<Exposer>> UiState::InnerMapType;

private:
    std::string m_name;
public:
    using UiObjectContainer::UiObjectContainer;
    UiState(std::string name, UiState::InnerMapType map = {}) :
        m_name(name),
        UiObjectContainer(map)
    {}

    std::string GetName() const {
        return m_name;
    }

    template <class T>
    T* as() const {
        //const T* result = dynamic_cast<const T*>(this);
        //if (!result) {
        //    throw std::runtime_error("Not in the correct state");
        //}
        return (T*)this;
    }
};

class SimpleUiState : public UiState {
public:
    SimpleUiState(std::string name, std::map<std::string, std::shared_ptr<Exposer>> map = {}) :
        UiState(name, map)
    {
    }
};

typedef TStaticValue<std::shared_ptr<UiState>> UiStateValue;

// Maybe remove when we really don't care about mdl logging anymore
void Write(IMDLBinaryFile* pmdlFile, const std::shared_ptr<UiState>& value);
ZString GetString(int indent, const std::shared_ptr<UiState>& value);
ZString GetFunctionName(const std::shared_ptr<UiState>& value);
typedef TModifiableValue<std::shared_ptr<UiState>, ModifiableUiStateName> UiStateModifiableValue;

template <typename EntryType>
class UiList : public Value {
private:
    std::vector<EntryType> m_list;

protected:
    std::vector<EntryType>& GetListInternal() {
        return m_list;
    }

public:
    template<class... T>
    UiList(std::vector<EntryType> list, T ... values) :
        m_list(list),
        Value(values...)
    {}

    const std::vector<EntryType>& GetList() {
        return m_list;
    }

    void Insert(int position, EntryType entry) {
        auto iterator = m_list.begin();
        std::advance(iterator, position);

        m_list.insert(iterator, entry);
        Changed();
    }

    void InsertAtEnd(EntryType entry) {
        m_list.insert(m_list.end(), entry);
        Changed();
    }

    void Remove(int position) {
        auto iterator = m_list.begin();
        std::advance(iterator, position);

        m_list.erase(iterator);
        Changed();
    }

    void RemoveAll() {
        m_list.clear();
        Changed();
    }
};

typedef UiList<TRef<UiObjectContainer>> ContainerList;
typedef UiList<TRef<Image>> ImageList;

template <typename T>
class TableUiList : public UiList<T> {
private:

public:
    TableUiList(sol::table table) : 
        UiList({})
    {
        std::vector<T> list;

        table.for_each([&list](const sol::object& key, const sol::object& value) {
            list.push_back(value.as<T>());
        });

        GetListInternal() = list;
    }
};

template <typename EntryType, typename ...Types>
class TransformedList : public UiList<EntryType> {

private:
    std::function<std::vector<EntryType>(Types...)> m_callback;

public:
    TransformedList(std::function<std::vector<EntryType>(Types...)> callback, TRef<TStaticValue<Types>>... values) :
        UiList<EntryType>({}),
        m_callback(callback)
    {
        AddChild(new CallbackWhenChanged<Types...>([this](Types... values) {
            GetListInternal() = m_callback(values...);
        }, values...));
    }
};

template <typename ResultEntryType, typename OriginalEntryType>
class MappedList : public UiList<ResultEntryType> {
private:
    std::function<ResultEntryType(OriginalEntryType, TRef<Number>)> m_callback;

public:
    MappedList(const TRef<UiList<OriginalEntryType>>& list, std::function<ResultEntryType(OriginalEntryType, TRef<Number>)> callback) :
        m_callback(callback),
        UiList({}, list)
    {
    }

    TRef<UiList<OriginalEntryType>> GetSourceList() {
        return (TRef<UiList<OriginalEntryType>>)(UiList<OriginalEntryType>*)GetChild(0);
    }

    void Evaluate() override {
        std::vector<ResultEntryType> list;
        int i = 0;
        for (auto entry : GetSourceList()->GetList()) {
            TRef<Number> index = new Number((float)i);

            list.push_back(m_callback(entry, index));
            ++i;
        }

        GetListInternal() = list;
    }
};

class CallbackSinkWithTRef : public CallbackSink {
private:
    std::vector<TRef<IObject>> m_vector;

public:
    CallbackSinkWithTRef(std::function<void()> callback) :
        CallbackSink(callback)
    {}

    void AddTRef(TRef<IObject> value) {
        m_vector.push_back(value);
    }

};

class SolEventSource : public EventSourceImpl {

public:
    SolEventSource() :
        EventSourceImpl()
    {
    }

    virtual ~SolEventSource() {
    }

    virtual sol::object MakeSolObject(sol::this_state L) = 0;
};

class SolEventSinkInitializer {
    typedef std::function<std::function<void()>(std::vector<sol::object>)> CallbackType;

private:
    CallbackType m_funcInitialize;

public:
    SolEventSinkInitializer(CallbackType funcCallback) :
        m_funcInitialize(funcCallback)
    {}

    TRef<IEventSink> Initialize(TRef<SolEventSource> psource, std::vector<sol::object> object) {
        auto sink = new CallbackSinkWithTRef(m_funcInitialize(object));

        //workaround
        sink->AddTRef(psource);

        return sink;
    }

    static std::shared_ptr<SolEventSinkInitializer> Create(CallbackType callbackInitializer) {
        return std::make_shared<SolEventSinkInitializer>(callbackInitializer);
    }

    static std::shared_ptr<SolEventSinkInitializer> CreateFromCallback(std::function<void()> callback) {
        return std::make_shared<SolEventSinkInitializer>([callback](std::vector<sol::object> obj) {
            return [callback]() {
                callback();
            };
        });
    }

    template <typename T>
    static std::shared_ptr<SolEventSinkInitializer> CreateFromModifiable(TRef<SimpleModifiableValue<T>> pvalue) {
        return std::make_shared<SolEventSinkInitializer>([pvalue](std::vector<sol::object> obj) {
            TRef<TStaticValue<T>> end = obj.at(0).as<TRef<TStaticValue<T>>>();
            return [pvalue, end]() {
                pvalue->SetValue(end->GetValue());
            };
        });
    }

    template <typename T>
    static std::shared_ptr<SolEventSinkInitializer> CreateFromSink(TRef<typename TEvent<T>::Sink> psink) {
        return std::make_shared<SolEventSinkInitializer>([psink](std::vector<sol::object> obj) {
            TRef<TStaticValue<T>> pvalue = obj.at(0).as<TRef<TStaticValue<T>>>();

            return [psink, pvalue]() {
                psink->OnEvent(nullptr, pvalue->GetValue());
            };
        });
    }
};

class SolEventSourceFiltered : public SolEventSource {
private:
    TRef<SolEventSource> m_psource;
    IEventSink* m_psink;
    TRef<Boolean> m_pactive;

public:
    SolEventSourceFiltered(TRef<SolEventSource> psource, TRef<Boolean> pactive) :
        m_psource(psource),
        m_pactive(pactive),
        SolEventSource(),
        m_psink(nullptr)
    {
        m_psink = new CallbackSink([this]() {
            if (this->m_pactive->GetValue()) {
                this->Trigger();
            }
        });
        m_psource->AddSink(m_psink);
    }

    ~SolEventSourceFiltered() override {
        if (m_psink) {
            m_psource->RemoveSink(m_psink);
        }
    }

    sol::object MakeSolObject(sol::this_state L) override {
        return m_psource->MakeSolObject(L);
    }
};

class SolEventSourceVoid : public SolEventSource {
private:
    TRef<IEventSource> m_psource;
    TRef<IEventSink> m_psink;

public:
    SolEventSourceVoid(TRef<IEventSource> source) :
        m_psource(source)
    {
        m_psink = new CallbackSink([this]() {
            this->Trigger();
        });
        source->AddSink(m_psink);
    }

    ~SolEventSourceVoid() {
        m_psource->RemoveSink(m_psink);
    }

    sol::object MakeSolObject(sol::this_state L) override {
        return sol::nil;
    }

    static TRef<SolEventSource> Create(TRef<IEventSource> source) {
        return new SolEventSourceVoid(source);
    }
};

template <typename T>
class SolEventSourceFromValue : public SolEventSource, public TEvent<T>::SourceImpl {
    typedef TRef<TStaticValue<T>> WrappedType;
private:
    WrappedType m_value;
    IEventSink* m_sink;

public:
    SolEventSourceFromValue() :
        m_value(WrappedType()),
        SolEventSource(),
        TEvent<T>::SourceImpl()
    {
        m_sink = new CallbackValueSink<T>([this](T value) {
            m_value->SetValue(value.as<T>());
            SolEventSource::Trigger();
        });
        TEvent<T>::SourceImpl::AddSink(m_sink)
    }

    ~SolEventSourceFromValue() {
        delete m_sink;
    }

    sol::object MakeSolObject(sol::this_state L) override {
        return sol::make_object<WrappedType>(L, m_value);
    }
};
