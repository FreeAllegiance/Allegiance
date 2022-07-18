
#pragma once

#include "items.hpp"

class FileNamespace {
public:
    static void AddNamespace(sol::state* m_pLua, Loader* pLoader) {
        sol::table table = m_pLua->create_table();
        table["LoadLua"] = [pLoader](std::string path) {
            return pLoader->LoadScript(path);
        };

        m_pLua->set("File", table);
    }
};