#ifndef __CATTLEDBUPDATE_H__
#define __CATTLEDBUPDATE_H__

typedef struct __tagDBUPDATE
{
	UINT uMajorVersion;
	UINT uMinorBersion;
	const char* pszUpdateStmts;
} DBUPDATE;


extern DBUPDATE _dbUpdates[];


#endif