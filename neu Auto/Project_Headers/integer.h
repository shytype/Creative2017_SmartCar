/*-------------------------------------------*/
/* Integer type definitions for FatFs module */
/*-------------------------------------------*/

#ifndef _FF_INTEGER
#define _FF_INTEGER

#ifdef _WIN32	/* FatFs development platform */

#include <windows.h>
#include <tchar.h>

#else			/* Embedded platform */

/* This type MUST be 8 bit */
#ifndef _TYPEDEF_BYTE_
#define _TYPEDEF_BYTE_
typedef unsigned char BYTE;
#endif

/* These types MUST be 16 bit */
#ifndef _TYPEDEF_SHORT_
#define _TYPEDEF_SHORT_
typedef signed short SHORT;
#endif

#ifndef _TYPEDEF_WORD_
#define _TYPEDEF_WORD_
typedef unsigned short WORD;
#endif

#ifndef _TYPEDEF_WCHAR_
#define _TYPEDEF_WCHAR_
typedef unsigned short	WCHAR;
#endif

/* These types MUST be 16 bit or 32 bit */
#ifndef _TYPEDEF_INT_
#define _TYPEDEF_INT_
typedef signed short INT;
#endif

#ifndef _TYPEDEF_UINT_
#define _TYPEDEF_UINT_
typedef unsigned short UINT;
#endif

/* These types MUST be 32 bit */
#ifndef _TYPEDEF_LONG_
#define _TYPEDEF_LONG_
typedef signed int LONG;
#endif

#ifndef _TYPEDEF_DWORD_
#define _TYPEDEF_DWORD_
typedef unsigned int DWORD;
#endif

#endif

#endif
