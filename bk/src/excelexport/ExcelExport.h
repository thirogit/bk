#ifndef _EXCELEXPORT_H_INCLUDED_
#define _EXCELEXPORT_H_INCLUDED_

#include "ExcelExportSettings.h"
#include <libxl.h>
#include <data\IOwnDoc.h>
#include <data\IHentsDoc.h>
#include <logic\herdregistry\ICowPresence.h>
#include "..\context\SeasonSession.h"

class ExcelExport
{
public:
	ExcelExport(SeasonSession* session,const ExcelExportSettings* settings);
	void Export(const std::wstring& xlsFile,const std::vector<IOutDoc*> & ownDocs);
	void Export(const std::wstring& xlsFile,const std::vector<IInDoc*> & inDocs);
	void Export(const std::wstring& xlsFile,const std::vector<IBuyDoc*> & buyDocs);
	void Export(const std::wstring& xlsFile, uint32_t viewHerdId, const std::vector<IMoveDoc*> & moveDocs);
	void Export(const std::wstring& xlsFile,const std::vector<ISellDoc*>& sellDocs);
	void Export(const std::wstring& xlsFile,const std::vector<ICowPresence*> & cows);
	void Export(const std::wstring& xlsFile, const std::vector<IInvoice*>& invoices);


	void ExportDocs(const std::wstring& xlsFile, const std::vector<IOutDoc*> & ownDocs);
	void ExportDocs(const std::wstring& xlsFile, const std::vector<IInDoc*> & inDocs);
	void ExportDocs(const std::wstring& xlsFile, const std::vector<IBuyDoc*> & buyDocs);
	void ExportDocs(const std::wstring& xlsFile, const std::vector<IMoveDoc*> & moveDocs);
	void ExportDocs(const std::wstring& xlsFile, const std::vector<ISellDoc*>& sellDocs);


	std::wstring GetFileExtension() const;
protected:	
	void WriteCows(libxl::Sheet* pSheet, const std::vector<ICowPresence*> & cows);
	void WriteDocs(libxl::Sheet* pSheet, const std::vector<IOutDoc*> & ownDocs);
	void WriteDocs(libxl::Sheet* pSheet, const std::vector<IInDoc*> & inDocs);
	void WriteDocs(libxl::Sheet* pSheet, const std::vector<IBuyDoc*> & buyDocs);
	void WriteDocs(libxl::Sheet* pSheet, const std::vector<IMoveDoc*> & moveDocs);
	void WriteDocs(libxl::Sheet* pSheet, const std::vector<ISellDoc*>& sellDocs);




	std::wstring PreparePath(const std::wstring& xlsFile);
	void SetupLicence(libxl::Book* book);
    
private:
	SeasonSession* m_session;
	const ExcelExportSettings* m_settings;	
};


#endif