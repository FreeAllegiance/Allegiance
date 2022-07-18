
#pragma once

#include "items.hpp"

class ColorNamespace {
public:
    static void AddNamespace(sol::state* m_pLua) {
        sol::table table = m_pLua->create_table();
        table["Create"] = [](UiType const& r, UiType const& g, UiType const& b, sol::optional<UiType> const& alpha) {
            TRef<Number> pWrappedAlpha;
            if (alpha) {
                pWrappedAlpha = CreateNumberFromUiType(alpha.value());
            } else {
                pWrappedAlpha = new Number(1.0f);
            }
            return UiType(ColorTransform::Create(
                CreateNumberFromUiType(r),
                CreateNumberFromUiType(g),
                CreateNumberFromUiType(b),
                pWrappedAlpha
            ));
        };

        m_pLua->set("Color", table);
    }
};
