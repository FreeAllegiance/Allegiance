#pragma once

#include <string>
#include <vector>

#include "tref.h"

class ProgressState {
public:
    virtual TRef<Number> GetProgress() {
        return new Number(0.0f);
    }

    virtual TRef<StringValue> GetErrorMessage() {
        return new StringValue("");
    }

    virtual TRef<Boolean> GetIsDone() {
        return NumberTransform::Equals(GetProgress(), new Number(1.0f));
    };

    virtual TRef<StringValue> GetStatusString() {
        return new TransformedValue<ZString, ZString, float>([](ZString strError, float fProgress) {
            if (strError != "") {
                return "Error (" + strError + ")";
            }

            if (fProgress == 1.0f) {
                return ZString("Complete (100%)");
            }

            return ZString(std::string("Running (" + std::to_string(fProgress * 100) + "%)").c_str());
        }, GetErrorMessage(), GetProgress());
    }
};

class SimpleProgressState : public ProgressState {
private:
    TRef<SimpleModifiableValue<float>> m_pProgress;
    TRef<SimpleModifiableValue<ZString>> m_pError;

public:
    SimpleProgressState() :
        m_pProgress(new SimpleModifiableValue<float>(0.0f)),
        m_pError(new SimpleModifiableValue<ZString>(""))
    {}

    TRef<Number> GetProgress() override {
        return m_pProgress;
    }

    TRef<StringValue> GetErrorMessage() override {
        return m_pError;
    }

    void SetProgress(float fProgress) {
        m_pProgress->SetValue(fProgress);
    }

    void SetDone() {
        m_pProgress->SetValue(1.0f);
    }

    void SetError(std::string msg) {
        m_pError->SetValue(msg.c_str());
    }
};

class FailedProgressState : public SimpleProgressState {
public:
    FailedProgressState(std::string msg) {
        SetError(msg);
    }
};

class ModConfigurationWrapper : public Value {
protected:
    TRef<UpdatingConfiguration> m_pconfiguration;
    std::string m_strFallbackName;

public:
    ModConfigurationWrapper(TRef<UpdatingConfiguration> pconfiguration, std::string strFallbackName) :
        Value(pconfiguration),
        m_pconfiguration(pconfiguration),
        m_strFallbackName(strFallbackName)
    {
    }

    TRef<SimpleModifiableValue<ZString>> GetName() {
        return m_pconfiguration->GetString("Name", m_strFallbackName);
    }

    TRef<SimpleModifiableValue<ZString>> GetSteamId() {
        return m_pconfiguration->GetString("SteamId", "");
    }

    TRef<SimpleModifiableValue<ZString>> GetAuthorSteamId() {
        return m_pconfiguration->GetString("AuthorSteamId", "");
    }

};

class Mod {
private:
    std::string m_strArtPath;

    TRef<ModConfigurationWrapper> m_pModConfiguration;

public:
    Mod(std::string strArtPath, std::string strFallbackName) :
        m_strArtPath(strArtPath)
    {
        m_pModConfiguration = new ModConfigurationWrapper(new UpdatingConfiguration(CreateJsonConfigurationStore(GetArtPath() + "\\mod.json")), strFallbackName);
    }

    std::string GetArtPath() {
        return m_strArtPath;
    }

    TRef<ModConfigurationWrapper> GetModConfiguration() {
        return m_pModConfiguration;
    }

    std::shared_ptr<ProgressState> UploadToWorkshop();

};

class ModdingEngine {
private:
    std::vector<std::shared_ptr<Mod>> m_vMods;

public:

    static const std::string GetPath();

    ModdingEngine(
        const std::vector<std::shared_ptr<Mod>>& vMods
    ) :
        m_vMods(vMods)
    {

    }

    static std::string ModNameToDirectoryName(std::string name);

    static std::shared_ptr<ModdingEngine> Create(TrekApp* papp);


    std::shared_ptr<ProgressState> SynchronizeWorkshopMods();

    const std::vector<std::shared_ptr<Mod>>& GetMods() {
        return m_vMods;
    }

    std::shared_ptr<Mod> CreateMod(std::string name);
};