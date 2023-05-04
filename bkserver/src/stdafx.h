#pragma once

#include "common_config.h"

#if defined WIN32 || defined _WIN64

#include "targetver.h"
//#include <stdio.h>
//#include <tchar.h>
//#include <afx.h>

#endif

#define WIDEN2(X) L ## X
#define WIDEN(X) WIDEN2(X)
#define __WFILE__ WIDEN(__FILE__)


