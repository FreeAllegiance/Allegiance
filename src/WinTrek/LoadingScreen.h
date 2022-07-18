#pragma once

#include "tref.h"
#include "Treki.h"

class LoadingScreen {
private:
    TRef<TrekApp> m_papp;
    TEvent<Time>::Cleanable m_cleanableOnFrame;
    TEvent<>::Cleanable m_cleanableOnClose;

public:
    LoadingScreen(TrekApp* papp) :
        m_papp(papp)
    {
    }

    ~LoadingScreen();

    void Start();
    void Terminate();

    void EvaluateFrame(Time time);
};