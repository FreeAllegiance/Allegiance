#include "pch.h"

#include "ModdingEngine.h"

#include "steamhelpers.h"
#include "Win32app.h"

const std::string ModdingEngine::GetPath() {
    return GetExecutablePath() + "\\Mods";
}

std::shared_ptr<ModdingEngine> ModdingEngine::Create(TrekApp* papp) {
    std::vector<std::shared_ptr<Mod>> vMods = {};

    //Go through the mod directory and add each directory in there

    std::string search_path = ModdingEngine::GetPath() + "\\*";
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && fd.cFileName[0] != '.') {
                vMods.push_back(std::make_shared<Mod>(ModdingEngine::GetPath() + "\\" + fd.cFileName, fd.cFileName));
            }
        } while (::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }

    return std::make_shared<ModdingEngine>(vMods);
}

class DownloadItemCallback : public SimpleProgressState {
private:
    PublishedFileId_t m_modSteamId;

    STEAM_CALLBACK(DownloadItemCallback, OnDownloadItemCallback, DownloadItemResult_t);

public:
    DownloadItemCallback(PublishedFileId_t modSteamId) :
        m_modSteamId(modSteamId)
    {
        SteamUGC()->DownloadItem(modSteamId, true);
    }

    PublishedFileId_t GetModSteamId() {
        return m_modSteamId;
    }
};
void DownloadItemCallback::OnDownloadItemCallback(DownloadItemResult_t* pResponse) {
    if (pResponse->m_unAppID != STEAM_APP_ID) {
        return;
    }
    if (pResponse->m_nPublishedFileId != m_modSteamId) {
        return;
    }

    if (pResponse->m_eResult != k_EResultOK) {
        debugf((const char*)(ZString("Error occured during downloading of mod ") + (std::to_string(m_modSteamId)).c_str()));
        SetError(("Error occured during downloading of mod ") + (std::to_string(m_modSteamId)));
    }

    SetDone();
}

std::shared_ptr<ProgressState> ModdingEngine::SynchronizeWorkshopMods()
{
    uint32 count = SteamUGC()->GetNumSubscribedItems();

    PublishedFileId_t* mod_ids = new PublishedFileId_t[count];
    
    count = SteamUGC()->GetSubscribedItems(mod_ids, count);
    for (int i = 0; i < count; ++i) {
        PublishedFileId_t id = mod_ids[i];
        std::shared_ptr<ProgressState> pModProgress = std::make_shared<DownloadItemCallback>(id);
    }

    delete[] mod_ids;
    return std::shared_ptr<ProgressState>();
}

std::string ModdingEngine::ModNameToDirectoryName(std::string name) {
    std::string s = name;

    //only alphanumeric
    std::replace_if(s.begin(), s.end(), [](char c) {
        if (isalnum(c) == false) {
            return true;
        }
        return false;
    }, '_');

    //to lowercase
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);

    //cut off after a certain size
    s = s.substr(0, 30);

    return s;
}

std::shared_ptr<Mod> ModdingEngine::CreateMod(std::string name)
{
    std::string strCleanedName = ModNameToDirectoryName(name);

    std::string strModPath = ModdingEngine::GetPath() + "\\" + strCleanedName;

    auto pmod = std::make_shared<Mod>(strModPath, strCleanedName);
    auto conf = pmod->GetModConfiguration();
    conf->GetName()->SetValue(name.c_str());

    //write the configuration, which also creates the directory
    conf->Update();

    m_vMods.push_back(pmod);
    return pmod;
}

void ActivateSteamOverlay(PublishedFileId_t steam_id) {
    SteamFriends()->ActivateGameOverlayToWebPage((std::string("steam://url/CommunityFilePage/") + std::to_string(steam_id)).c_str());
}

class CreateModCallback : public SteamCallback<CreateItemResult_t> {
private:
    PublishedFileId_t m_modSteamId;

public:
    CreateModCallback() :
        m_modSteamId(0),
        SteamCallback(SteamUGC()->CreateItem(STEAM_APP_ID, k_EWorkshopFileTypeCommunity))
    {}

    void OnSuccess(CreateItemResult_t* pResponse) {
        if (pResponse->m_eResult != k_EResultOK) {
            debugf("CreateItem had an error: %d", (int)pResponse->m_eResult);
            return;
        }

        //we do not activate the steam overlay here since we always immediately do an upload which checks the m_bUserNeedsToAcceptWorkshopLegalAgreement flag

        m_modSteamId = pResponse->m_nPublishedFileId;
    }

    PublishedFileId_t GetModSteamId() {
        return m_modSteamId;
    }
};

class UpdateModProgress : public SimpleProgressState, public SteamCallback<SubmitItemUpdateResult_t> {
private:
    UGCUpdateHandle_t m_handleUpdate;

    TRef<SimpleModifiableValue<ZString>> m_pStageString;

    TEvent<Time>::Cleanable m_cleanableOnFrame;

protected:
    void OnSuccess(SubmitItemUpdateResult_t* pResponse) override {
        m_cleanableOnFrame.Cleanup();
        if (pResponse->m_eResult != k_EResultOK) {
            debugf("SubmitItem had an error: %d", (int)pResponse->m_eResult);
            SetError("Mod update failed: " + std::to_string((int)pResponse->m_eResult));
            return;
        }

        if (pResponse->m_bUserNeedsToAcceptWorkshopLegalAgreement == true) {
            ActivateSteamOverlay(pResponse->m_nPublishedFileId);
        }
        else {
            //always
            ActivateSteamOverlay(pResponse->m_nPublishedFileId);
        }

        SetDone();
    }

    void OnError(SubmitItemUpdateResult_t* pResponse) override {
        m_cleanableOnFrame.Cleanup();
        SetError("Mod update failed");
    }

public:
    UpdateModProgress(UGCUpdateHandle_t handle) :
        m_handleUpdate(handle),
        m_pStageString(new SimpleModifiableValue<ZString>("")),
        SteamCallback(SteamUGC()->SubmitItemUpdate(handle, NULL))
    {
        m_cleanableOnFrame = std::move(GetEngineWindow()->GetEvaluateFrameEventSource()->AddSinkManaged(new CallbackValueSink<Time>([this](Time time) {
            uint64 bytesProcessed;
            uint64 bytesTotal;
            SteamUGC()->GetItemUpdateProgress(m_handleUpdate, &bytesProcessed, &bytesTotal);

            if (bytesTotal > 0) {
                this->SetProgress((float)bytesProcessed / bytesTotal);
            }

            return true;
        })));
    }
};

std::shared_ptr<ProgressState> Mod::UploadToWorkshop()
{
    //check if mod exists already
    TRef<SimpleModifiableValue<ZString>> pModSteamId = m_pModConfiguration->GetSteamId();
    bool bIsNew = pModSteamId->GetValue() == "";
    if (bIsNew) {
        // this mod is new or unknown, create it

        auto callbackCreate = std::make_shared<CreateModCallback>();
        callbackCreate->Wait();

        if (callbackCreate->IsSuccess() == false || callbackCreate->GetModSteamId() == 0) {
            return std::make_shared<FailedProgressState>("An error occured while attempting to create the mod on steam");
        }

        pModSteamId->SetValue(std::to_string(callbackCreate->GetModSteamId()).c_str());
        m_pModConfiguration->GetAuthorSteamId()->SetValue(std::to_string(SteamUser()->GetSteamID().ConvertToUint64()).c_str());
    }

    //write the configuration
    m_pModConfiguration->Update();

    //update the steam mod
    UGCUpdateHandle_t hUpdate = SteamUGC()->StartItemUpdate(STEAM_APP_ID, std::stol(std::string(pModSteamId->GetValue())));

    SteamUGC()->SetItemTitle(hUpdate, m_pModConfiguration->GetName()->GetValue());
    SteamUGC()->SetItemContent(hUpdate, m_strArtPath.c_str());
    if (bIsNew) {
        SteamUGC()->SetItemVisibility(hUpdate, k_ERemoteStoragePublishedFileVisibilityPrivate);
    }

    //upload
    return std::make_shared<UpdateModProgress>(hUpdate);
}
