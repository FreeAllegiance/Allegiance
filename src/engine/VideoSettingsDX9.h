#ifndef _VIDEOSETTINGS_H_
#define _VIDEOSETTINGS_H_

#include <windows.h>
#include <zstring.h>

#include "Configuration.h"

class EngineConfigurationWrapper : public Value {
protected:
    TRef<UpdatingConfiguration> m_pconfiguration;

public:
    EngineConfigurationWrapper(TRef<UpdatingConfiguration> pconfiguration) :
        Value(pconfiguration),
        m_pconfiguration(pconfiguration)
    {
    }

    TRef<SimpleModifiableValue<bool>> GetGraphicsFullscreen() {
        return m_pconfiguration->GetBool("Graphics.Fullscreen", false);
    }

    TRef<SimpleModifiableValue<float>> GetGraphicsResolutionX() {
        return m_pconfiguration->GetInt("Graphics.ResolutionX", m_pconfiguration->GetIntValue("CombatFullscreenXSize", 0));
    }

    TRef<SimpleModifiableValue<float>> GetGraphicsResolutionY() {
        return m_pconfiguration->GetInt("Graphics.ResolutionY", m_pconfiguration->GetIntValue("CombatFullscreenYSize", 0));
    }

    TRef<SimpleModifiableValue<bool>> GetGraphicsUseVSync() {
        return m_pconfiguration->GetBool("Graphics.UseVSync", m_pconfiguration->GetBoolValue("UseVSync", true));
    }

    TRef<SimpleModifiableValue<bool>> GetGraphicsUseAntiAliasing() {
        return m_pconfiguration->GetBool("Graphics.UseAntiAliasing", m_pconfiguration->GetBoolValue("UseAntialiasing", false));
    }

    TRef<SimpleModifiableValue<float>> GetGraphicsMaxTextureSizeLevel() {
        return m_pconfiguration->GetInt("Graphics.MaxTextureSizeLevel", m_pconfiguration->GetIntValue("MaxTextureSize", 3));
    }

    TRef<SimpleModifiableValue<float>> GetGraphicsGamma() {
        return m_pconfiguration->GetFloat("Graphics.Gamma", m_pconfiguration->GetFloatValue("Gamma", 1.13f));
    }

    TRef<SimpleModifiableValue<bool>> GetDebugLogToOutput() {
        return m_pconfiguration->GetBool("Debug.LogToOutput", m_pconfiguration->GetBoolValue("OutputDebugString", true));
    }

    TRef<SimpleModifiableValue<bool>> GetDebugLogToFile() {
        return m_pconfiguration->GetBool("Debug.LogToFile", m_pconfiguration->GetBoolValue("LogToFile", false));
    }

    TRef<SimpleModifiableValue<bool>> GetDebugMdl() {
        return m_pconfiguration->GetBool("Debug.Mdl", false);
    }

    TRef<SimpleModifiableValue<bool>> GetDebugWindow() {
        return m_pconfiguration->GetBool("Debug.Window", false);
    }

    TRef<SimpleModifiableValue<bool>> GetDebugLua() {
        return m_pconfiguration->GetBool("Debug.Lua", false);
    }

    TRef<SimpleModifiableValue<bool>> GetMouseUseRawInput() {
        return m_pconfiguration->GetBool("Mouse.UseRawInput", false);
    }
};

//Imago added bool bStartFullscreen, bool bRaise 6/29/09, only prompts when "Safe Mode" and restored the -windowed command line switch
//Rock: Removed bRaise, maybe reintroduce when it would be used again. Config instead of registry path.
bool PromptUserForVideoSettings(bool bStartFullscreen, int iAdapter, HINSTANCE hInstance, PathString & szArtPath, const TRef<EngineConfigurationWrapper>& config );

#endif // _VIDEOSETTINGS_H_
