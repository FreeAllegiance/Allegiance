#include "pch.h"

#include <inttypes.h> 

#include "steamhelpers.h"

#include "CallsignTagInfo.h"

CallsignHandler::CallsignHandler(const TRef<GameConfigurationWrapper>& pconfiguration, const std::vector<std::shared_ptr<CallsignSquad>>& squads) :
    m_pconfiguration(pconfiguration),
    m_squads(squads)
{
}

ZString CallsignHandler::CleanupCallsign(ZString callsign)
{
    std::string s = std::string(callsign);

    s.erase(std::remove_if(s.begin(), s.end(), [](char c) {
        if (c == '_') {
            return false;
        }
        if (isalnum(c) == false) {
            return true;
        }
        return false;
    }), s.end());

    //if it's too small, do something with it
    if (s.size() <= 2) {
        s = std::string("Pilot_") + s;
    }

    //cut off after a certain size. 24 is the max for the full callsign
    s = s.substr(0, 16);

    return s.c_str();
}

std::vector<std::shared_ptr<CallsignSquad>> CallsignHandler::GetAvailableSquads()
{
    return m_squads;
}

std::shared_ptr<CallsignSquad> CallsignHandler::GetSquadForTag(ZString tag)
{
    if (tag == "") {
        return nullptr;
    }

    std::vector<ZString> vectorAllowedTokens;
    for (const std::shared_ptr<CallsignSquad>& squad : GetAvailableSquads()) {
        if (squad->GetCleanedTag() == tag) {
            return squad;
        }
    }

    return nullptr;
}

TRef<StringValue> CallsignHandler::GetCleanedFullCallsign()
{
    return new TransformedValue<ZString, ZString, ZString, ZString>([this](ZString strCallsign, ZString strSquad, ZString strToken) {
        ZString strCallsignPart = CallsignHandler::CleanupCallsign(strCallsign);

        ZString strSquadPart = "";
        std::vector<ZString> vectorAllowedTokens;

        std::shared_ptr<CallsignSquad> squad = GetSquadForTag(strSquad);

        if (squad != nullptr) {
            std::vector<ZString> vectorSquadTokens;
            vectorAllowedTokens.insert(vectorAllowedTokens.end(), std::begin(squad->GetAvailableOfficerTokens()), std::end(squad->GetAvailableOfficerTokens()));
            strSquadPart = "@" + strSquad;
        }

        ZString strTokenPart = "";
        if (strToken != "") {
            for (ZString allowedToken : vectorAllowedTokens) {
                if (allowedToken == strToken) {
                    strTokenPart = strToken;
                }
            }
        }

        return strTokenPart + strCallsignPart + strSquadPart;
    }, m_pconfiguration->GetOnlineCharacterName(), m_pconfiguration->GetOnlineSquadTag(), m_pconfiguration->GetOnlineOfficerToken());
}

ZString FixupCallsignTag(ZString callsignTag)
{
    ZString returnValue = callsignTag;

    // It's tradition!
    if (returnValue == "MsAlleg")
        return "Alleg";

    // Give our older members a nice badge of honor.
    if (returnValue == "AllegFAO")
        return "Vet";

    // Allows us to have some group tags that may have already been claimed.
    if (returnValue.Find("@") >= 0)
        returnValue = returnValue.RightOf("@");

    if (returnValue.Find("#") >= 0)
        returnValue = returnValue.RightOf("#");

    if (returnValue.Find("!") >= 0)
        returnValue = returnValue.RightOf("!");

    if (returnValue.Find("&") >= 0)
        returnValue = returnValue.RightOf("&");

    if (returnValue.Find("%") >= 0)
        returnValue = returnValue.RightOf("%");

    if (returnValue.Find("^") >= 0)
        returnValue = returnValue.RightOf("^");

    if (returnValue.Find("*") >= 0)
        returnValue = returnValue.RightOf("*");

    if (returnValue.Find("$") >= 0)
        returnValue = returnValue.RightOf("$");

    if (returnValue == "Alleg" || returnValue == "Vet")
        return "Fail";

    return returnValue;
}

ZString CallsignSquad::CleanupSquadTag(ZString tag)
{
    std::string s = std::string(tag);

    s.erase(std::remove_if(s.begin(), s.end(), [](char c) {
        if (isalnum(c) == false) {
            return true;
        }
        return false;
    }), s.end());

    //cut off after a certain size, 24 is the max for the full callsign. 
    // 16 reserved for the non-squad part of the callsign. minus one for the token. minus one for the @. So a max of 6
    s = s.substr(0, 6);

    return ZString(s.c_str());
}

class OfficierCallbackClass : public SteamCallback<ClanOfficerListResponse_t>{
    CSteamID m_idSteamClan;

public:
    OfficierCallbackClass(CSteamID idSteamClan) :
        m_idSteamClan(idSteamClan),
        SteamCallback(SteamFriends()->RequestClanOfficerList(m_idSteamClan))
    {
    }

    void OnSuccess(ClanOfficerListResponse_t* pResponse) {
        if (pResponse->m_bSuccess == false) {
            debugf("RequestClanOfficerList had an error");
        }
    }

    int GetClanOfficerCount() {
        return SteamFriends()->GetClanOfficerCount(m_idSteamClan);
    }

    CSteamID GetClanOfficerByIndex(int index) {
        return SteamFriends()->GetClanOfficerByIndex(m_idSteamClan, index);
    }

    CSteamID GetClanOwner() {
        return SteamFriends()->GetClanOwner(m_idSteamClan);
    }
};

class SteamCallsignSquad : public CallsignSquad {
private:
    CSteamID m_SteamClanId;
    CSteamID m_SteamUserId;

    std::shared_ptr<OfficierCallbackClass> m_pcallback;

    ZString m_tag;
    bool m_bTokensLoaded;
    std::vector<ZString> m_availableTokens;

    const std::shared_ptr<OfficierCallbackClass>& GetCallback() {
        if (!m_pcallback) {
            m_pcallback = std::make_shared<OfficierCallbackClass>(m_SteamClanId);
            m_pcallback->Wait();
        }
        return m_pcallback;
    }

public:
    SteamCallsignSquad(CSteamID steamClanId, CSteamID steamUserId) :
        m_SteamClanId(steamClanId),
        m_SteamUserId(steamUserId),
        m_bTokensLoaded(false),
        m_availableTokens({})
    {
        m_tag = FixupCallsignTag(SteamFriends()->GetClanTag(m_SteamClanId));
    }

    const std::vector<ZString>& GetAvailableOfficerTokens() override {
        if (!m_bTokensLoaded) {

            bool bIsOfficer = false;

            int officerCount = GetCallback()->GetClanOfficerCount();
            for (int j = 0; j < officerCount; j++)
            {
                if (GetCallback()->GetClanOfficerByIndex(j) == m_SteamUserId)
                {
                    bIsOfficer = true;
                    break;
                }
            }

            // If the user is the group owner, then they are an officer.
            if (m_SteamUserId == GetCallback()->GetClanOwner())
            {
                bIsOfficer = true;
            }

            if (m_tag == "Dev" || m_tag == "Alleg")
            {
                bIsOfficer = true;
            }

            m_availableTokens = std::vector<ZString>({});
            if (bIsOfficer) {
                m_availableTokens.push_back("*");
                m_availableTokens.push_back("^");
            }

            m_bTokensLoaded = true;
        }

        return m_availableTokens;
    }

    const ZString GetCleanedTag() {
        return m_tag;
    };
};

std::shared_ptr<CallsignSquad> CreateSquadFromSteam(CSteamID squadSteamClanId, CSteamID userSteamId)
{
    return std::make_shared<SteamCallsignSquad>(squadSteamClanId, userSteamId);
}

std::shared_ptr<CallsignHandler> CreateCallsignHandlerFromSteam(const TRef<GameConfigurationWrapper>& pconfiguration)
{
    int addedItemCount = 0;

    CSteamID currentUser;
    if (SteamUser() != nullptr)
        currentUser = SteamUser()->GetSteamID();

    int nGroupsFound = SteamFriends()->GetClanCount();
    int nGroups = std::min(1000, nGroupsFound); //only do the first n clans

    debugf("Finding squad tags. %d groups found, iterating over %d groups (possibly limit reached)", nGroupsFound, nGroups);

    std::vector<std::shared_ptr<CallsignSquad>> vSquads;
    for (int i = 0; i < nGroups; ++i)
    {
        CSteamID groupSteamID = SteamFriends()->GetClanByIndex(i);
        ZString szGroupName = SteamFriends()->GetClanName(groupSteamID);
        ZString szGroupTag = SteamFriends()->GetClanTag(groupSteamID);

        debugf("Finding squad tags. Found name=%s, tag=%s", (const char*)szGroupName, (const char*)szGroupTag);

        // Don't do the expensive officer call unless there is actually a tag associated with the group.
        if (szGroupTag.GetLength() == 0) {
            debugf("Finding squad tags. Tag empty, skipping");
            continue;
        }

        std::shared_ptr<CallsignSquad> squad = CreateSquadFromSteam(groupSteamID, currentUser);
        if (squad->GetCleanedTag() == "") {
            debugf("Finding squad tags. Cleaned up tag empty, skipping");
            continue;
        }

        debugf("Finding squad tags. Added tag. fixed_tag=%s, available tokens determined lazily", (const char*)squad->GetCleanedTag());

        vSquads.push_back(squad);
    }

    return std::make_shared<CallsignHandler>(pconfiguration, vSquads);
}
