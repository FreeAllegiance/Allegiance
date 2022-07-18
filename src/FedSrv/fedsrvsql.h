//#ifndef ALLSRV_STANDALONE Imago 6/10 resurrected

// fedsrvsql.h
// curtc
// sql stuff 

#define CSTATEMENTS 100 // we will ZAssert if we add more statements than defined here
#define CCOLPARMMAX 20 // same for columns or parameters (combined)
enum SQLPARM
{
  SQL_IN_PARM,    // input parameters to sql statements OR stored procs
  SQL_OUT_PARM,   // output parameter to sql statement, NOT stored procedure
  SQL_OUT_PROC,   // stored proc return values or stored proc out-only parameters
  SQL_INOUT_PROC  // stored proc in/out parameters
};

class ISQLSite : public IObject
{
public:
  virtual int OnMessageBox(const char * strText, const char * strCaption, UINT nType) = 0;
};

SQLHSTMT AddStatement(SQLCHAR * scSQL);
int AddCol(void * pvBuff, SQLSMALLINT ssiCType, SQLPARM parmtype, int cbBuff);
SQLRETURN SqlGo(SQLHSTMT hstmt);
SQLRETURN SqlGetRow(SQLHSTMT hstmt);
int InitSql(char * szRegKey, ISQLSite * pSQLSite);
void ShutDownSQL();

#define BEGIN_SQL_DEF(SQLSITE) \
  const int fooinitsql = InitSql(HKLM_FedSrv, SQLSITE); // defining variable just so we can call function

#define END_SQL_DEF

#define DEF_STMT(NAME, SQL) \
  SQLHSTMT g_hstmt##NAME = AddStatement((SQLCHAR *) SQL); 

#define SQL_PARM(STORAGE, NAME, CTYPE, INOUT) \
  STORAGE NAME = AddCol(&NAME, CTYPE, INOUT, sizeof(STORAGE)) ? NAME : NAME; // big stupid hack to always call AddCol

#define SQL_STR_PARM(NAME, LENGTH, INOUT) \
  SQLCHAR NAME [LENGTH + 1] = {AddCol(NAME, SQL_C_CHAR, INOUT, LENGTH + 1)};

#define SQL_INT4_PARM(NAME, INOUT)\
  SQL_PARM(SQLINTEGER, NAME, SQL_C_LONG, INOUT)

#define SQL_INT2_PARM(NAME, INOUT) \
  SQL_PARM(SQLSMALLINT, NAME, SQL_C_SHORT, INOUT)

#define SQL_INT1_PARM(NAME, INOUT) \
  SQL_PARM(SQLCHAR, NAME, SQL_C_TINYINT, INOUT)

#define SQL_FLT4_PARM(NAME, INOUT) \
  SQL_PARM(SQLREAL, NAME, SQL_C_FLOAT, INOUT)

#define SQL_FLT8_PARM(NAME, INOUT) \
  SQL_PARM(SQLFLOAT, NAME, SQL_C_DOUBLE, INOUT)

#define SQL_DATE_PARM(NAME, INOUT) \
  SQL_PARM(TIMESTAMP_STRUCT, NAME, SQL_C_TIMESTAMP, INOUT)

// Binary doesn't quite fit the pattern
#define SQL_BIN_PARM(NAME, LENGTH, INOUT) \
  SQLCHAR NAME[LENGTH] = {(char) AddCol(NAME, SQL_C_BINARY, INOUT, sizeof(NAME))};

#define SQL_GO(NAME) \
  SqlGo(g_hstmt##NAME)

#define SQL_GETROW(NAME) \
  SqlGetRow(g_hstmt##NAME)

#define FEDSTR(str) "ISNULL(RTRIM(" #str "), '~')"
#define FEDNUM(num) "ISNULL(" #num ", -1)"

void SqlStrCpy(char * szDest, SQLCHAR * szSrc, unsigned int cbSrc);

// but all the sql strings are fixed length, so give them a shortcut
#define SQLSTRCPY(szDest, szSrc) SqlStrCpy((szDest), (szSrc), sizeof(szSrc))


//#endif // !ALLSRV_STANDALONE
