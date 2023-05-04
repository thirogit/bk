#include "stdafx.h"
#include "ExcelExport.h"
#include "../consts.h"
#include "../text/CowTextProvider.h"

CExcelExport::CExcelExport() 
{
}

bool CExcelExport::Export(CDocDataArray &docs,const ExcelExportSettings& settings,const CString &xlsFile)
{
	ASSERT(docs.GetSize() > 0);
	int docsize = docs.GetSize(),cowsize = 0,i,j,dot;
	CString saveFile;
	Doc *pDoc = NULL;
	const char xlsExt[] = ".xls";

	i = xlsFile.ReverseFind('\\');
	dot = xlsFile.ReverseFind('.');
	if(dot < 0)
	{
		saveFile = xlsFile + xlsExt;
	}
	else
	{
		if(dot < i)
			saveFile = xlsFile + xlsExt;
		else
			saveFile = xlsFile;		
	}

	DeleteFile(saveFile);

	libxl::Book* book = xlCreateBook();
	
	if(settings.GetPutDocsInSeperateSheet())
	{
		
		for(i = 0;i < docsize;i++)
		{
			pDoc = docs[i];

			libxl::Sheet* sheet = book->addSheet(pDoc->GetDocNoStr());
			WriteCows(sheet,settings,pDoc->GetCows());
			
		}
	}
	else
	{
		CCowArray cows;
		for(i = 0;i < docsize;i++)
		{
			pDoc = docs[i];			
			CCowArray::ArrayIterator cowIt = pDoc->GetCows()->iterator();
			while (cowIt.hasNext())
			{
				cows.Add((*(cowIt++)));				
			}			
		}

		libxl::Sheet* sheet = book->addSheet("BK");
		WriteCows(sheet,settings,CowArrayPtr(&cows));
	}
	 book->save(saveFile);
	 book->release();
	return true;
}
void CExcelExport::WriteCows(libxl::Sheet* pSheet,const ExcelExportSettings& settings,CowArrayPtr cows)
{
	CowTextProvider cowText;
	Cow* pCow;
	CCowArray::ArrayIterator cowIt = cows->iterator();
	int row = 1;
	while(cowIt.hasNext())
	{
		CStringArray rowValues;
		pCow = (*(cowIt++));
		for(int field = 0,count = settings.GetFieldCount();field < count;field++)
		{				
			pSheet->writeStr(row, field,cowText.GetDataText(pCow,settings.GetField(field))); 		
		}		
		row++;
	}
}
/*
void CExcelExport::SetHeaders(CSpreadSheet& sheet,const ExcelExportSettings& settings)
{
	CStringArray aHeaders;
	for(int field = 0,count = settings.GetFieldCount();field < count;field++)
	{
		aHeaders.Add(CString());
		
	}
	sheet.AddHeaders(aHeaders);
}
*/