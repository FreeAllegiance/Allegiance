#pragma once

#include "igc.h"

class NativeClientClusterSite : public ClusterSite
{
public:
	NativeClientClusterSite(IclusterIGC * pcluster)
		:
		m_pcluster(pcluster)
	{
	}

	virtual void                    AddScanner(SideID   sid, IscannerIGC* scannerNew)
	{
		ZAssert(sid >= 0);
		ZAssert(sid < c_cSidesMax);
		ZAssert(scannerNew);

		AddIbaseIGC((BaseListIGC*)&(m_scanners[sid]), scannerNew);
	}
	virtual void                    DeleteScanner(SideID   sid, IscannerIGC* scannerOld)
	{
		ZAssert(sid >= 0);
		ZAssert(sid < c_cSidesMax);
		ZAssert(scannerOld);

		DeleteIbaseIGC((BaseListIGC*)&(m_scanners[sid]), scannerOld);
	}
	virtual const ScannerListIGC*      GetScanners(SideID   sid) const
	{
		ZAssert(sid >= 0);
		ZAssert(sid < c_cSidesMax);

		return &(m_scanners[sid]);
	}

	virtual void Terminate()
	{
		ClusterSite::Terminate();
	}

private:
	IclusterIGC * m_pcluster;
	ScannerListIGC  m_scanners[c_cSidesMax];
};

