#ifndef _EXCELEXPORT_H_INCLUDED_
#define _EXCELEXPORT_H_INCLUDED_

//#include "SpreadSheet.h"
#include "../datatypes/DocDataArray.h"
#include "ExcelExportSettings.h"
#include "libxl.h"

class CExcelExport
{
public:
	CExcelExport();
	bool Export(CDocDataArray &docs,const ExcelExportSettings& settings,const CString &xlsFile);
private:
	
	void WriteCows(libxl::Sheet* pSheet,const ExcelExportSettings& settings,CowArrayPtr cows);
    //void SetHeaders(Sheet* pSheet,const ExcelExportSettings& settings);
};


#endif