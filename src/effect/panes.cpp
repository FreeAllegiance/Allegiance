#include "pch.h"

/////////////////////////////////////////////////////////////////////////////
//
// Gauge Pane
//
/////////////////////////////////////////////////////////////////////////////

GaugePane::GaugePane(Surface*         psurface,
                     const Color&     colorFlash,
                     float            minValue,
                     float            maxValue,
                     const Color&     colorEmpty)
:
    m_psurface(psurface),
    m_colorFlash(colorFlash),
    m_colorEmpty(colorEmpty),
    m_minValue(minValue),
    m_maxValue(maxValue),
    m_value(0),
    m_valueOld(0),
    m_valueFlash(0),
    m_timeLastChange(Time::Now())
{
    ZAssert(m_psurface);
    ZAssert(m_minValue < m_maxValue);

    InternalSetSize(m_psurface->GetRect().Size());
}

void GaugePane::Paint(Surface* psurface)
{
    if (m_value != 0) {
        psurface->BitBlt(
            WinPoint(0, 0),
            m_psurface,
            WinRect(0, 0, m_value, YSize())
        );
    }

    if (m_value < m_valueFlash) {
        psurface->FillRect(
            WinRect(
                m_value,
                0,
                m_valueFlash,
                YSize()
            ),
            m_colorFlash
        );
    }
}

void GaugePane::SetValue(float v, bool fFlash)
{
    m_value =
        (int)bound(
            (v - m_minValue) * ((float)XSize()) / (m_maxValue - m_minValue),
            0.0f,
            (float)XSize()
        );
}

void GaugePane::Update(Time time)
{
    if (m_value != m_valueOld) {
        if (m_value < m_valueOld) {
            m_valueFlash = m_valueOld;
        } else {
            m_valueFlash = m_value;
        }

        m_timeLastChange = time;
        m_valueOld = m_value;

        NeedPaint();
    }

    if (m_value != m_valueFlash && time - m_timeLastChange > 0.25f) {
        m_valueFlash = m_value;
        NeedPaint();
    }
}
