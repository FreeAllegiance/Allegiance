/*-------------------------------------------------------------------------
 * src\FedSrv\SQLHELP.CPP
 * 
 * Helper functions for sql stuff
 * 
 * Owner: 
 * 
 * Copyright 1986-1998 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

//
// Okay, I know this seems weird, but it was the best way I could do it.  Besides,
// this code is going to be replaced soon anyway.
//
#ifdef ALLCLUB_MODULE 
#define _FEDSRV_PCH_  // exclude including pch.h for ALLCLUB
#endif

#include "pch.h"

//#if !defined(ALLSRV_STANDALONE) Imago 6/10

#include <sql.h>
#include <sqlext.h>

SQLHENV hSqlEnv = 0; // the environment for all our odbc stuff
SQLHDBC hSqlDbc = 0; // the database connection
SQLHSTMT g_rghstmt[CSTATEMENTS]; 
SQLCHAR * g_rgscstmt[CSTATEMENTS]; 
SQLSMALLINT * g_rgssiColType[CSTATEMENTS]; 
SQLSMALLINT * g_rgssiParmType[CSTATEMENTS]; 
int g_iSql = 0; \
int g_iCol = 0; \
int g_iParm = 0;
TRef<ISQLSite> g_pSQLSite;
bool g_fSQLRetry = false;
int g_cSQLRetries = 0;

SQLHSTMT g_hstmt_Last = 0;

/*-------------------------------------------------------------------------
 * SQLWhatsWrong
 *-------------------------------------------------------------------------
 * Purpose:
 *    Diagnostics for odbc calls
 * 
 * Side Effects:
 *    Breaks on anything but SQL_SUCCESS
 */
void SQLWhatsWrong(SQLSMALLINT ssiHandleType, SQLHANDLE sh)
{
  SQLCHAR scState[5];
  SQLINTEGER siNativeError;
  SQLCHAR scMsgText[512];
  SQLSMALLINT cbMsgText;
  SQLRETURN sqlret = SQLGetDiagRec(ssiHandleType, sh, 1, scState, &siNativeError, 
      scMsgText, sizeof(scMsgText), &cbMsgText);
  ZDebugOutput((char *) scMsgText);

  // BIG hack to work with zone profile
  if (7312 == siNativeError)
    return;

  // If our query is deadlocked, let's try again
  if (1205 == siNativeError)
  {
    debugf("Query deadlocked. Retry #%d.", g_cSQLRetries);
    g_fSQLRetry = true;
    g_cSQLRetries++;
    SQLCloseCursor(g_hstmt_Last); // we get an invalid cursor state without this.
    Sleep(50 * g_cSQLRetries);
    return;
  }
  
  if(g_pSQLSite) // sometimes NULL because g_pSQLSite is sometimes constructed (and set to NULL) sometime after InitSql() has its way with it.
    g_pSQLSite->OnMessageBox((char *) scMsgText, "FedSrv SQL Error", MB_OK | MB_ICONINFORMATION);
  else 
  {
      debugf("SQL Error %s\n", scMsgText);
      printf("SQL Error %s\n", scMsgText);
  }
  // if we can't access the database, we're screwed, so give up.
  // TODO: Make the exit more graceful
  ZAssert(0); // let's at least break in debug build
  exit(EXIT_FAILURE);
}


/*-------------------------------------------------------------------------
 * AddStatement
 *-------------------------------------------------------------------------
 * Purpose:
 *    initializes a sql statement and keeps track of it for auto cleanup
 */
SQLHSTMT AddStatement(SQLCHAR * scSQL)
{
  if (g_iSql >= CSTATEMENTS)
  {
    OutputDebugString("Too many SQL statements. Increment CSTATEMENTS\n");
    DebugBreak();
    exit(0);
  }

  SQLRETURN sqlret;
  sqlret = SQLAllocHandle(SQL_HANDLE_STMT, hSqlDbc, &g_rghstmt[g_iSql]);
  if (SQL_SUCCESS != sqlret)
    SQLWhatsWrong(SQL_HANDLE_STMT, g_rghstmt[g_iSql]);
  
  sqlret = SQLPrepare(g_rghstmt[g_iSql], scSQL, SQL_NTS);
  if (SQL_SUCCESS != sqlret)
    SQLWhatsWrong(SQL_HANDLE_STMT, g_rghstmt[g_iSql]);

  g_iCol = 0;
  g_iParm = 0;
  return g_rghstmt[g_iSql++];
}


/*-------------------------------------------------------------------------
 * AddCol
 *-------------------------------------------------------------------------
 * Purpose:
 *    bind a variable to a column of the sql statement
 * 
 * Parameters:
 *    pvBuff: where to bind the column data to
 * 
 * Returns:
 *    0. Returns a value only so we can call it during global initialization,
 *        before any code blocks execute.
 */
int AddCol(void * pvBuff, SQLSMALLINT ssiCType, SQLPARM parmtype, int cbBuff)
{
  SQLRETURN sqlret;
  static SQLINTEGER cbWhoCares;

  switch (parmtype)
  {
    case SQL_OUT_PARM:
      sqlret = SQLBindCol(g_rghstmt[g_iSql - 1], ++g_iCol, ssiCType, pvBuff, 
                          cbBuff, &cbWhoCares);
      if (SQL_SUCCESS != sqlret)
        SQLWhatsWrong(SQL_HANDLE_STMT, g_rghstmt[g_iSql - 1]);
      break;
    
    case SQL_IN_PARM:
      // we're using the same type for C and SQL, so they must match
      sqlret = SQLBindParameter(g_rghstmt[g_iSql - 1], ++g_iParm, SQL_PARAM_INPUT, 
          ssiCType, ssiCType, cbBuff, 0, pvBuff, cbBuff, NULL);
      break;

    case SQL_OUT_PROC: //out value from stored procedure
      // we're using the same type for C and SQL, so they must match
      sqlret = SQLBindParameter(g_rghstmt[g_iSql - 1], ++g_iParm, SQL_PARAM_OUTPUT, 
          ssiCType, ssiCType, cbBuff, 0, pvBuff, cbBuff, &cbWhoCares);
      break;

    case SQL_INOUT_PROC: //out value from stored procedure
      // we're using the same type for C and SQL, so they must match
      sqlret = SQLBindParameter(g_rghstmt[g_iSql - 1], ++g_iParm, SQL_PARAM_INPUT_OUTPUT, 
          ssiCType, ssiCType, cbBuff, 0, pvBuff, cbBuff, &cbWhoCares);
      break;
  }

  return 0;
}


/*-------------------------------------------------------------------------
 * ParseCommandLine
 *-------------------------------------------------------------------------
 * Purpose:
 *    find information needed from the command line
 * 
 * Returns:
 *    0. Returns a string, the contents of which are stored in a static 
 *       buffer. Multiple calls overwrite the buffer.
 *       If the desired key is not in the command line, then we check to
 *       see if a default value is supplied. If not, the registry is
 *       checked for a default value. If it's not in the registry, we get a
 *       empty result.
 */
 char*  ParseCommandLine (char * szRegKey, char* szParameterName, char* szDefault = 0)
 {
    static  char    szBuffer[64];
    LPTSTR          szCommandLine = GetCommandLine ();
    char*           location = strstr (szCommandLine, szParameterName);
    szBuffer[0] = 0;
    if (location)
    {
        location += strlen (szParameterName);
        ZAssert (*location == '=');
        location++;
        char*   szBufferPtr = szBuffer;
        while (!isspace (*location))
            *szBufferPtr++ = *location++;
        *szBufferPtr = 0;
    }
    else if (szDefault)
    {
        strcpy (szBuffer, szDefault);
    }
    else
    {
        HKEY hKey;
        DWORD dw;
        DWORD cb = sizeof(szBuffer);
        if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, szRegKey, 0, KEY_READ, &hKey))
            RegQueryValueEx (hKey, szParameterName, NULL, &dw, (LPBYTE)szBuffer, &cb);
        RegCloseKey(hKey);
    }
    return szBuffer;
 }

/*-------------------------------------------------------------------------
 * InitSql
 *-------------------------------------------------------------------------
 * Purpose:
 *    Set up the odbc stuff
 * 
 * Returns:
 *    0. Returns a value only so we can call it during global initialization,
 *        before any code blocks execute.
 */
int InitSql(char * szRegKey, ISQLSite * pSQLSite)
{
  g_pSQLSite = pSQLSite;
  
  // Setup odbc stuff
  SQLRETURN sqlret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hSqlEnv);
  if (SQL_SUCCESS != sqlret)
    SQLWhatsWrong(SQL_HANDLE_ENV, hSqlEnv);
  
  sqlret = SQLSetEnvAttr(hSqlEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC2, 0);
  if (SQL_SUCCESS != sqlret)
    SQLWhatsWrong(SQL_HANDLE_ENV, hSqlEnv);
  
  sqlret = SQLAllocHandle(SQL_HANDLE_DBC, hSqlEnv, &hSqlDbc);
  if (SQL_SUCCESS != sqlret)
    SQLWhatsWrong(SQL_HANDLE_DBC, hSqlDbc);

#if 0
  SQLCHAR StrConnectionOut[1<<10];
  SQLSMALLINT cbStrConnectionOut;
  sqlret = SQLDriverConnect(hSqlDbc, NULL, (SQLCHAR*) 
                            "DRIVER={SQL Server};SERVER=FEDSRV", SQL_NTS, 
                            StrConnectionOut, sizeof(StrConnectionOut), 
                            &cbStrConnectionOut, SQL_DRIVER_NOPROMPT);
#else
  char szUser[64];
  char szPW[64];
  char szDatabase[64];
  strcpy (szUser, ParseCommandLine (szRegKey, "SQLUser"));
  strcpy (szPW, ParseCommandLine (szRegKey, "SQLPW"));
  strcpy (szDatabase, ParseCommandLine (szRegKey, "SQLSrc", "Federation"));
  printf ("  Connecting to DSN (%s) as user (%s) with password (%s)...\n", szDatabase, szUser, szPW);
  sqlret = SQLConnect(hSqlDbc, (SQLCHAR*) szDatabase, SQL_NTS, (SQLCHAR*) szUser, SQL_NTS, (SQLCHAR*) szPW, SQL_NTS);
#endif
  
  if (SQL_SUCCESS != sqlret && SQL_SUCCESS_WITH_INFO != sqlret)
    SQLWhatsWrong(SQL_HANDLE_DBC, hSqlDbc);
  
   SQLCHAR scODBCver[6];
  SQLSMALLINT cbODBCver;
  sqlret = SQLGetInfo(hSqlDbc, SQL_DRIVER_ODBC_VER, scODBCver, sizeof(scODBCver), &cbODBCver);
  if (SQL_SUCCESS != sqlret)
    SQLWhatsWrong(SQL_HANDLE_DBC, hSqlDbc);

  return 0;
}

/*-------------------------------------------------------------------------
 * ShutDownSQL
 *-------------------------------------------------------------------------
 * Purpose:
 *    Clean up odbc stuff
 */
void ShutDownSQL()
{
  int iHStmt;
  for (iHStmt = 0; iHStmt < g_iSql; iHStmt++)
    SQLFreeHandle(SQL_HANDLE_STMT, g_rghstmt[g_iSql]);

  SQLDisconnect(hSqlDbc);
  SQLFreeHandle(SQL_HANDLE_DBC, hSqlDbc);
  SQLFreeHandle(SQL_HANDLE_ENV, hSqlEnv);
}


/*-------------------------------------------------------------------------
 * SqlGo
 *-------------------------------------------------------------------------
 * Purpose:
 *    Execute a prepared sql statement
 */
SQLRETURN SqlGo(SQLHSTMT hstmt)
{
  SQLRETURN sqlret;
  static CTempTimer tt("in SqlGo", .05f);
  tt.Start();

  if (g_hstmt_Last)
    SQLCloseCursor(g_hstmt_Last); // close cursor from previous execution, which may or may not exist, no harm if no cursor
  g_hstmt_Last = hstmt;
  g_fSQLRetry = false;
  g_cSQLRetries = 0;
  do
  {
    sqlret = SQLExecute(hstmt);
    if (SQL_SUCCESS != sqlret)
      SQLWhatsWrong(SQL_HANDLE_STMT, hstmt);
  } while (g_fSQLRetry);
  tt.Stop();
  return sqlret;
}


SQLRETURN SqlGetRow(SQLHSTMT hstmt)
{
  SQLRETURN sqlret;
  static CTempTimer tt("in SqlGetRow", .01f);
  tt.Start();
  sqlret = SQLFetch(hstmt);
  
  if (SQL_SUCCESS != sqlret && SQL_NO_DATA != sqlret)
    SQLWhatsWrong(SQL_HANDLE_STMT, hstmt);
  tt.Stop();
  return sqlret;
}


/*-------------------------------------------------------------------------
 * SqlStrCpy
 *-------------------------------------------------------------------------
 * Purpose:
 *    Copy a string returned from SQL, taking into consideration the lameness
 *    that we can't get back a real null string.
 */
void SqlStrCpy(char * szDest, SQLCHAR * szSrc, unsigned int cbSrc)
{
  if ('~' == szSrc[0] && '\0'== szSrc[1]) // null string
    *szDest = '\0';
  else
    CopyMemory(szDest, szSrc, cbSrc);
}


//#endif // !defined(ALLSRV_STANDALONE)
