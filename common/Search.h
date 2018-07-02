/*
///////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   Search.h
//  Version     :   1.0
//  Creator     :   RM
//  Create Date :   2006-01-01
//  Comment     :
//
///////////////////////////////////////////////////////////////////////////////
*/

#ifndef __SEARCH_H__
#define __SEARCH_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*
///////////////////////////////////////////////////////////////////////////////
// D E F I N E S
///////////////////////////////////////////////////////////////////////////////
*/

/*
///////////////////////////////////////////////////////////////////////////////
// S T R U C T U R E S
///////////////////////////////////////////////////////////////////////////////
*/

/*
///////////////////////////////////////////////////////////////////////////////
// G L O B A L S
///////////////////////////////////////////////////////////////////////////////
*/

/*
///////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////
*/
ULONG SearchString( PCOMMON_STACK_U Result, LPVOID StartAddress, LPVOID EndAddress, const unsigned char *String, ULONG Length );
ULONG SearchStringEx( PCOMMON_STACK_U Result, HANDLE Process, LPVOID StartAddress, LPVOID EndAddress, const unsigned char *String, ULONG Length );
ULONG SearchStringAndLog( LPVOID StartAddress, LPVOID EndAddress, const unsigned char *String, ULONG Length );
ULONG SearchStringAndLogEx( HANDLE Process, LPVOID StartAddress, LPVOID EndAddress, const unsigned char *String, ULONG Length );

#ifdef __cplusplus
}
#endif
/*
///////////////////////////////////////////////////////////////////////////////
// E N D  O F  F I L E
///////////////////////////////////////////////////////////////////////////////
*/
#endif /* __SEARCH_H__ */