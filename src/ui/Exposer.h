#pragma once

#include "tref.h"
#include "value.h"
#include "color.h"

#include "ui_sol.h"

#include <variant>
#include <stdexcept>

struct UiType;
class StringDumper {
public:
    static std::string dump(const UiType& value);
};

//the compiler version on the build server is slightly out of date
//https://stackoverflow.com/questions/50510122/stdvariant-with-overloaded-lambdas-alternative-with-msvc
#if false
template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...)->overload<Ts...>;
#else
template<class...Ts>
struct overloaded_t {};

template<class T0>
struct overloaded_t<T0> :T0 {
    using T0::operator();
    overloaded_t(T0 t0) :T0(std::move(t0)) {}
};
template<class T0, class T1, class...Ts>
struct overloaded_t<T0, T1, Ts...> :T0, overloaded_t<T1, Ts...> {
    using T0::operator();
    using overloaded_t<T1, Ts...>::operator();
    overloaded_t(T0 t0, T1 t1, Ts... ts) :
        T0(std::move(t0)),
        overloaded_t<T1, Ts...>(std::move(t1), std::move(ts)...) {
    }
};

template<class...Ts>
overloaded_t<Ts...> overload(Ts...ts) { return { std::move(ts)... }; }
#endif

struct UiType {
    typedef std::variant<std::monostate, int, float, std::string, std::vector<UiType>, TRef<ColorValue>> VariantType;
    VariantType m_data;

    UiType(VariantType data) : 
        m_data(data) 
    {
    }

    template <typename T>
    T get() const {
        const T* value = std::get_if<T>(&m_data);
        if (value == nullptr) {
            throw std::runtime_error("Invalid argument type, was: " + StringDumper::dump(*this));
        }
        return *value;
    }

    template <typename R, typename ...Types>
    R typeSwitch(Types... cases) const {
        return std::visit(overload(
            cases...,
            [&](auto const& x) {
                throw std::runtime_error("Invalid argument type, was: " + StringDumper::dump(*this));
                return R();
            }
        ), m_data);
    }
};

TRef<Number> CreateNumberFromUiType(UiType const& obj);

class Exposer {
protected:
    std::shared_ptr<void> m_pobject;

public:
    Exposer(std::shared_ptr<void>& pobject) : m_pobject(pobject) {
    }

    template <typename T>
    Exposer(T object) {
        m_pobject = std::make_shared<T>(object);
    }

    virtual sol::object ExposeSolObject(lua_State* L) = 0;

    template <typename T>
    operator const T&() const {
        return *std::static_pointer_cast<T>(m_pobject);
    }
};

template <typename T>
class TypeExposer : public Exposer {
    //T m_object;

public:
    TypeExposer(const T& obj) : Exposer(obj) {
    }

    static std::shared_ptr<TypeExposer<T>> Create(const T& obj) {
        return std::shared_ptr<TypeExposer<T>>(new TypeExposer(obj));
    }

    sol::object ExposeSolObject(lua_State* L) {
        return sol::make_object<T>(L, *std::static_pointer_cast<T>(m_pobject));
    }

    operator T() const {
        return m_object;
    }
};

template <typename T>
class TStaticValueExposer : public TypeExposer<TRef<TStaticValue<T>>> {
    typedef TRef<TStaticValue<T>> ExposedType;

public:
    TStaticValueExposer(const ExposedType& obj) : TypeExposer(obj) {
    }

    static std::shared_ptr<TStaticValueExposer<T>> CreateStatic(const T& value) {
        return std::shared_ptr<TStaticValueExposer<T>>(new TStaticValueExposer<T>(new TStaticValue<T>(value)));
    }

    //static std::shared_ptr<TStaticValueExposer<T>> CreateModifiable(const T& value) {
    //    return std::shared_ptr<T>(new TypeExposer(TRef(new TModifiableValue<T>(value))));
    //}
};

typedef TStaticValueExposer<bool> BooleanExposer;
typedef TStaticValueExposer<float> NumberExposer;
typedef TStaticValueExposer<ZString> StringExposer;

class SolEventSinkInitializer;
typedef TypeExposer<std::shared_ptr<SolEventSinkInitializer>> EventSinkInitializerExposer;

class Exposed {
    sol::object m_object;
    sol::state m_lua;

public:
    template <typename T>
    Exposed(T arg) {
        m_object = sol::make_object<T>(m_lua, arg);
    }

    sol::object GetSolObject(lua_State* L) {
        return sol::make_object<sol::object>(L, m_object);
    }

};