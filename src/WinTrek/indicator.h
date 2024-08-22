#pragma once

//////////////////////////////////////////////////////////////////////////////
//
// Indicator Images
//
//////////////////////////////////////////////////////////////////////////////

TRef<Image> CreateIndicatorImage(Modeler* pmodeler, Viewport* pviewport, Number* ptime);

//////////////////////////////////////////////////////////////////////////////
//
// Training Overlay
//
//////////////////////////////////////////////////////////////////////////////
enum TrainingOverlay
{
    NoTrainingOverlay,
    SpeedTrainingOverlay,
    MotionIndicatorTrainingOverlay,
    ReticuleTrainingOverlay,
    ThrottleTrainingOverlay,
    FuelTrainingOverlay,
    EnergyTrainingOverlay,
    AmmoTrainingOverlay,
    HullTrainingOverlay,
    ShieldTrainingOverlay,
    ChatTrainingOverlay,
    SectorTrainingOverlay,
    InventoryTrainingOverlay,
    TargetTrainingOverlay,
    CommandTrainingOverlay,
    EyeballTrainingOverlay,
    KillBonusTrainingOverlay,
    RollTrainingOverlay,
    BoosterTrainingOverlay,
    TargetCenterTrainingOverlay,
    FireGunsTrainingOverlay,
    TargetEnemyTrainingOverlay,
    MissileTrainingOverlay,
    CloakTrainingOverlay
};
void        SetTrainingOverlay(TrainingOverlay overlay);
inline
void        ClearTrainingOverlay(void) { SetTrainingOverlay(NoTrainingOverlay); }