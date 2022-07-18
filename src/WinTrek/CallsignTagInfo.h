#pragma once

#include "WinTrek.H"

class CallsignSquad {

public:
    static ZString CleanupSquadTag(ZString tag);

    virtual const std::vector<ZString>& GetAvailableOfficerTokens() = 0;

    virtual const ZString GetCleanedTag() = 0;
};

std::shared_ptr<CallsignSquad> CreateSquadFromSteam(CSteamID squadSteamClanId, CSteamID userSteamId);

class CallsignHandler {
private:
    TRef<GameConfigurationWrapper> m_pconfiguration;
    std::vector<std::shared_ptr<CallsignSquad>> m_squads;

    static ZString CleanupCallsign(ZString callsign);

public:
    CallsignHandler(const TRef<GameConfigurationWrapper>& pconfiguration, const std::vector<std::shared_ptr<CallsignSquad>>& squads);

    std::vector<std::shared_ptr<CallsignSquad>> GetAvailableSquads();

    std::shared_ptr<CallsignSquad> GetSquadForTag(ZString tag);

    TRef<StringValue> GetCleanedFullCallsign();

    static bool IsValidCallsign(ZString callsign);
};

std::shared_ptr<CallsignHandler> CreateCallsignHandlerFromSteam(const TRef<GameConfigurationWrapper>& pconfiguration);

