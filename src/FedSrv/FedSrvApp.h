#pragma once

/*-------------------------------------------------------------------------
 * FedSrvApp
 *-------------------------------------------------------------------------
 * Purpose:
 *    This class is called by the ZAssert code when an ZAssert happens
 */
#define FED_DEBUG_DEBUGOUT 1
#define FED_DEBUG_FILE     2

// mmf log to file on SRVLOG define as well as _DEBUG
#ifdef _DEBUG
#define SRVLOG
#endif

#ifdef SRVLOG // mmf changed this from _DEBUG

  class FedSrvApp : public Win32App 
  {
  public:
    FedSrvApp(void)
    :
        m_hFile(NULL),
        m_dwDebug(FED_DEBUG_DEBUGOUT)
    {
    }

    ~FedSrvApp(void)
    {
        CloseLogFile();
    }

    virtual void OpenLogFile(void)
    {
        CloseLogFile();
        // mmf server logfilename set here 
		// mmf changed to use logfile mechanism from client (Win32app.cpp)
        // char    bfr[200];
		// GetModuleFileName(NULL, bfr, 150);
        // char*   p = strrchr(bfr, '.');
        // if (p)
        //    *(p + 1) = '\0';
        // //_itoa(Time::Now().clock(), bfr + strlen(bfr), 10);
		// strcat(bfr, "txt");

		time_t  longTime;
        time(&longTime);
        tm*             t = localtime(&longTime);

        char    logFileName[MAX_PATH + 16];
        GetModuleFileName(NULL, logFileName, MAX_PATH);
        char*   p = strrchr(logFileName, '\\');
        if (!p)
			p = logFileName;
        else
			p++;

        const char* months[] = {"jan", "feb", "mar", "apr",
                                "may", "jun", "jul", "aug",
                                "sep", "oct", "nov", "dec"};
        strcpy(p, months[t->tm_mon]);
        sprintf(p + 3, "%02d%02d%02d%02d.txt",
                t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

        m_hFile = CreateFile(logFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NOT_CONTENT_INDEXED | FILE_FLAG_OVERLAPPED, NULL);
        ZAssert (m_hFile);

        m_overlapped.Offset = 0;
        m_overlapped.OffsetHigh = 0;

        m_overlapped.hEvent = NULL;
        m_nOffset = 0;
    }

    virtual void CloseLogFile(void)
    {
        if (m_hFile)
        {
            CloseHandle(m_hFile);
            m_hFile = NULL;
        }
    }

    virtual void DebugOutput(const char *psz);
    virtual bool OnAssert(const char* psz, const char* pszFile, int line, const char* pszModule);
    virtual void OnAssertBreak();

    void SetDebug(DWORD dw)
    {
      if (!(m_dwDebug & FED_DEBUG_FILE) && (dw & FED_DEBUG_FILE))
        OpenLogFile();
      else if ((m_dwDebug & FED_DEBUG_FILE) && !(dw & FED_DEBUG_FILE))
        CloseLogFile();
      char buff[640];
      wsprintf(buff, "Debug level set from %u to %u.\n", m_dwDebug, dw);
      m_dwDebug = dw;
      DebugOutput(buff);
    }
      
    DWORD GetDebug()
    {
        return m_dwDebug;
    }
      

  private:
    void AsyncFileOut(const char *psz);
    
      HANDLE        m_hFile;
      OVERLAPPED    m_overlapped;
      DWORD         m_dwDebug; // See FED_* above
    #pragma pack(push, 4)
      LONG          m_nOffset;
    #pragma pack(pop)
  
  };

  extern FedSrvApp g_app;

#endif
