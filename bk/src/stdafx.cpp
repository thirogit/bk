
// stdafx.cpp : source file that includes just the standard includes
// bk.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "App.h"

ServiceRegistry* GetServiceRegistry()
{
	return ((CBKApp*)AfxGetApp())->GetServiceRegistry();
}