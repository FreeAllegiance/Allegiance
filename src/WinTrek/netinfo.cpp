#include "pch.h"

#define MAX_STRING 1024

//////////////////////////////////////////////////////////////////////////////
//
// Network card info
//
//////////////////////////////////////////////////////////////////////////////

//========================================================================
//
// GetDeviceValue
//
// read	a value	from the HW	or SW of a PnP device
//
// in:
//	szHardwareKey	the hardware key
//	szKey			the sub-key
//	szValue			the value to query
//	cbbuf			the size of the output buffer
//
// out:
//	buf				the destination buffer
//	
// returns:
//	success status
//
//========================================================================

BOOL WINAPI _GetDeviceValue(LPCSTR szHardwareKey, LPCSTR szKey,	LPCSTR szValue,	BYTE* buf, DWORD cbbuf)
{
	HKEY	hkeyHW;
	HKEY	hkeySW;
	BOOL	f =	FALSE;
	DWORD	cb;
	char	szSoftwareKey[MAX_STRING];

	//
	// open	the	HW key
	//
	if (RegOpenKey(HKEY_LOCAL_MACHINE, szHardwareKey, &hkeyHW) == ERROR_SUCCESS)
	{
		//
		// try to read the value from the HW key
		//
		*buf = 0;
		cb = cbbuf;
		if (RegQueryValueEx(hkeyHW,	szValue, NULL, NULL, buf, &cb) == ERROR_SUCCESS)
		{
			f =	TRUE;
		}
		else
		{
			//
			// now try the SW key
			//
			static char	szSW[] = "System\\CurrentControlSet\\Services\\Class\\";

			lstrcpy(szSoftwareKey, szSW);
			cb = sizeof(szSoftwareKey) - sizeof(szSW);
			RegQueryValueEx(hkeyHW,	"Driver", NULL,	NULL, (BYTE	*)&szSoftwareKey[sizeof(szSW) -	1],	&cb);

			if (szKey)
			{
				lstrcat(szSoftwareKey, "\\");
				lstrcat(szSoftwareKey, szKey);
			}

			if (RegOpenKey(HKEY_LOCAL_MACHINE, szSoftwareKey, &hkeySW) == ERROR_SUCCESS)
			{
				*buf = 0;
				cb = cbbuf;
				if (RegQueryValueEx(hkeySW,	szValue, NULL, NULL, buf, &cb) == ERROR_SUCCESS)
				{
					f =	TRUE;
				}

				RegCloseKey(hkeySW);
			}
		}

		RegCloseKey(hkeyHW);
	}

	return f;
}

//========================================================================
//
// FindDevice
//
// enum	the	started	PnP	devices	looking	for	a device of	a particular class
//
//	iDevice			what device	to return (0= first	device,	1=second et)
//	szDeviceClass	what class device (ie "Display") NULL will match all
//	szDeviceID		buffer to return the hardware ID (MAX_STRING bytes)
//
// return TRUE if a	device was found.
//
// example:
//
//		for	(int i=0; FindDevice(i,	"Display", DeviceID); i++)
//		{
//		}
//
//========================================================================

BOOL WINAPI _FindDevice(int iDevice, LPCSTR szDeviceClass, LPSTR szHardwareKey, BOOL bIgnoreProblems)
{
	HKEY	hkeyPnP;
	HKEY	hkey;
	DWORD	n;
	DWORD	cb;
	DWORD	dw;
	char	ach[MAX_STRING];

	if (RegOpenKey(HKEY_DYN_DATA, "Config Manager\\Enum", &hkeyPnP)	!= ERROR_SUCCESS)
		return FALSE;

	for	(n=0; RegEnumKey(hkeyPnP, n, ach, sizeof(ach)) == 0; n++)
	{
		static char	szHW[] = "Enum\\";

		if (RegOpenKey(hkeyPnP,	ach, &hkey)	!= ERROR_SUCCESS)
			continue;

		lstrcpy(szHardwareKey, szHW);
		cb = MAX_STRING -	sizeof(szHW);
		RegQueryValueEx(hkey, "HardwareKey", NULL, NULL, (BYTE*)szHardwareKey +	sizeof(szHW) - 1, &cb);

		dw = 0;
		cb = sizeof(dw);
		RegQueryValueEx(hkey, "Problem", NULL, NULL, (BYTE*)&dw, &cb);
		RegCloseKey(hkey);

		if ((!bIgnoreProblems) && (dw != 0))		// if this device has a	problem	skip it
			continue;

		if (szDeviceClass)
		{
			_GetDeviceValue(szHardwareKey, NULL,	"Class", (BYTE *)ach, sizeof(ach));

			if (lstrcmpi(szDeviceClass,	ach) !=	0)
				continue;
		}

		//
		// we found	a device, make sure	it is the one the caller wants
		//
		if (iDevice-- == 0)
		{
			RegCloseKey(hkeyPnP);
			return TRUE;
		}
	}

	RegCloseKey(hkeyPnP);
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////
//
// Network card info
//
//////////////////////////////////////////////////////////////////////////////

ZString GetRegistryString(HKEY hkey, const ZString& strName)
{
    DWORD cb = MAX_STRING;
    BYTE pb[MAX_STRING];

    pb[0] = 0;

	ZVerify(RegQueryValueEx(hkey, strName, NULL, NULL, pb, &cb) == ERROR_SUCCESS);

    return ZString((PCC)pb);
}

bool CheckDevice(int index, bool& bValid, ZString& strDriverURL)
{
	char szDevice[MAX_STRING];
    bValid = false;

	if (_FindDevice(index, "Net", szDevice, FALSE)) {
        //
        // Is this the bad driver?
        //

        BYTE pb[MAX_STRING];

	    if (!_GetDeviceValue(szDevice, NULL, "DeviceDesc", pb, sizeof(pb))) {
            bValid = true;
            return true;
        };

        enum {
            dtRTL8139,
            dtRTL8129,
            dtRTL8029,
            dtPNPNT
        } driverType;


        if (_stricmp((PCC)pb, "rtl8139.sys") == 0) {
            driverType = dtRTL8139;
        }
        else if (_stricmp((PCC)pb, "rtl8129.sys") == 0) {
            driverType = dtRTL8129;
        }
        else if (_stricmp((PCC)pb, "rtl8029.sys") == 0) {
            driverType = dtRTL8029;
        }
        else if (_stricmp((PCC)pb, "pnpnt.sys") == 0) {
            driverType = dtPNPNT;
        }
	    else {
            bValid = true;
            return true;
        };

        //
        // Get the driver version number
        //

	    if (!_GetDeviceValue(szDevice, NULL, "Driver", pb, sizeof(pb))) {
            return true;
        }

        HKEY hkey;
        ZString strKey = "System\\CurrentControlSet\\Services\\Class\\" + ZString((PCC)pb);
        if (RegOpenKey(HKEY_LOCAL_MACHINE, strKey, &hkey) != ERROR_SUCCESS) {
            return true;
        }

        ZString strDeviceVxDs = GetRegistryString(hkey, "DeviceVxDs");
        //data.m_strNTMPDriver = GetRegistryString(hkey, "NTMPDriver");
        //data.m_strDriverDate = GetRegistryString(hkey, "DriverDate");

        char pszWindowsPath[MAX_PATH];
        GetWindowsDirectory(pszWindowsPath, sizeof(pszWindowsPath));

        ZString strDriverPath = ZString(pszWindowsPath) + "\\system\\" + strDeviceVxDs;

        DWORD dwTemp;
        DWORD dwVerInfoSize = GetFileVersionInfoSize((char*)(PCC)strDriverPath, &dwTemp);

        if (dwVerInfoSize == 0) {
            ZError("This should never happen");
            bValid = false;
        } else {
            void *pvVerInfo = _alloca(dwVerInfoSize);
            ZVerify(GetFileVersionInfo((char*)(PCC)strDriverPath, NULL, dwVerInfoSize, pvVerInfo));

            VS_FIXEDFILEINFO *lpvsFixedFileInfo = NULL;
            unsigned uTemp;
            ZVerify(VerQueryValue(pvVerInfo, "\\", (LPVOID*) &lpvsFixedFileInfo, &uTemp));

            WORD ver1 = HIWORD(lpvsFixedFileInfo->dwFileVersionMS);
            WORD ver2 = LOWORD(lpvsFixedFileInfo->dwFileVersionMS);
            WORD ver3 = HIWORD(lpvsFixedFileInfo->dwFileVersionLS);
            WORD ver4 = LOWORD(lpvsFixedFileInfo->dwFileVersionLS);

            switch (driverType)
            {
            case dtRTL8139:
                if (ver1 > 5 || ver1 == 5 && ver2 >= 373)
                    bValid = true;
                else
                    strDriverURL = "http://www.realtek.com.tw";
                break;

            case dtRTL8129:
                if (ver1 > 5 || ver1 == 5 && ver2 >= 368)
                    bValid = true;
                else
                    strDriverURL = "http://www.realtek.com.tw";
                break;

            case dtRTL8029:
                if (ver1 > 5 || ver1 == 5 && ver2 >= 505)
                    bValid = true;
                else
                    strDriverURL = "http://www.realtek.com.tw";
                break;

            case dtPNPNT: {
                    const char* szRealTek = "REALTEK";
                    const char* szCompanyName;
                    ZVerify(VerQueryValue(pvVerInfo, "\\StringFileInfo\\040904b0\\CompanyName", (LPVOID*) &szCompanyName, &uTemp));

                    if (strncmp(szCompanyName, szRealTek, sizeof(szRealTek)) != 0) {
                        bValid = true;
                        break;
                    }

                    OSVERSIONINFO osversioninfo;
                    osversioninfo.dwOSVersionInfoSize = sizeof(osversioninfo);                
                    GetVersionEx(&osversioninfo);

                    if (osversioninfo.dwPlatformId != VER_PLATFORM_WIN32_WINDOWS || osversioninfo.dwMajorVersion != 4){
                        bValid = true;
                        break;
                    }

                    if (osversioninfo.dwMinorVersion == 0) // if Win95
                    {
                        // at least 4.0.1381.1
                        if (ver1 > 4 || ver1 == 4 && (ver2 > 0 || ver2 == 0 && (ver3 > 1381 || ver3 == 1381 && ver4 >=1)))
                            bValid = true;
                        else
                            strDriverURL = "http://www.realtek.com.tw";
                    }
                    else
                    {
                        // Win98, at least ver 3.19.98.901
                        if (ver1 > 3 || ver1 == 3 && (ver2 > 19 || ver2 == 19 && (ver3 > 98 || ver3 == 98 && ver4 >= 901)))
                            bValid = true;
                        else
                            strDriverURL = "http://www.realtek.com.tw";
                    }
                }
                break;

            default:
                ZAssert(0);
            }
        }

        return true;
    }

    return false;
}

bool CheckNetworkDevices(ZString& strDriverURL)
{
    int  index = 0;
    bool bValid;

    while (CheckDevice(index, bValid, strDriverURL))
    {
        if (!bValid) {
            return false;
        }

        index++;
    }

    return true;
}


