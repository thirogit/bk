#include "stdafx.h"
#include "ContextAccess.h"
#include "App.h"

namespace ContextAccess
{
	XContext* GetXContext()
	{
		return ((CBKApp*)AfxGetApp())->GetContext();
	}
}