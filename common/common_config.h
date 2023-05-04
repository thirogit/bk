#pragma once

#if defined _MSC_VER
#define MSVC 1

#define strncasecmp(a,b,n) _strnicmp(a,b,n) 
#define widetoi(str) _wtoi(str)
#define vsnwprint_safe _vsnwprintf

#else

#include <stdlib.h>
#define sprintf_safe snprintf
#define widetoi(str) wcstol(str,NULL,10)
#define vsnwprint_safe vswprintf
#define USE_UTF8CPP

#endif
