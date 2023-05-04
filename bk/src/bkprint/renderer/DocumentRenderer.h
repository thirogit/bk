#ifndef __DOCUMENTRENDERER_H__
#define __DOCUMENTRENDERER_H__

#include "PrintOut.h"
#include "ExpandableTemplateDataSource.h"

namespace bkprint
{
	class DocumentRenderer 
	{		
	public:
		virtual PrintOut Print(ExpandableTemplateDataSource* dataSrc) = 0;
	};
}
#endif