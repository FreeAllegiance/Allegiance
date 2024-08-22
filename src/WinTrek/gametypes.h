#pragma once

/////////////////////////////////////////////////////////////////////////////
//
// ImageStringPane 
//
/////////////////////////////////////////////////////////////////////////////
class GameType : public IObject
{
public:
    virtual ZString GetName() = 0;
    virtual bool IsGameType(const MissionParams& misparams) = 0;
    virtual void Apply(MissionParams& misparams) = 0;

    static TRef<GameType> FindType(const MissionParams& misparams);
    static TList<TRef<GameType> >& GetGameTypes();
};