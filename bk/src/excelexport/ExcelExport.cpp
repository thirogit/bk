#include "stdafx.h"
#include "ExcelExport.h"
#include <filesystem/paths.h>
#include <string/IntString.h>
#include "../utils/StringLoader.h"
#include <string/TextUtils.h>
#include "../resource.h"
#include <boost/foreach.hpp>
#include "../text/DocNoText.h"
#include "../text/InvoiceNoText.h"
#include "../header/CowPresenceListHeader.h"
#include "../header/OwnDocsListHeader.h"
#include "../header/HentsDocsListHeader.h"
#include "../header/MoveDocsListHeader.h"

#include "../text/InvoiceViewText.h"
#include "../text/CowPresenceViewText.h"
#include "../text/OwnDocViewText.h"
#include "../text/HentsDocViewText.h"
#include "../text/MoveDocViewText.h"

#include "../view/InvoiceViewCreator.h"
#include "../view/CowPresenceViewCreator.h"
#include "../view/OwnDocViewCreator.h"
#include "../view/HentsDocViewCreator.h"
#include "../view/MoveDocViewCreator.h"



static const wchar_t _xls_ext[] = L"xls";

ExcelExport::ExcelExport(SeasonSession* session,const ExcelExportSettings* settings) : m_settings(settings), m_session(session)
{
}

std::wstring ExcelExport::PreparePath(const std::wstring& xlsFile)
{
	return xlsFile;
	/*if (paths_DoesFileExist(xlsFile))
	{		
		return xlsFile;
	}
	
	if (paths_DoesPathExist(xlsFile))
	{

	}

	std::wstring filename = paths_GetFileName(xlsFile);
	std::wstring directory = paths_GetDirectory(xlsFile);
	std::wstring fileext = paths_GetFileExt(filename);
	fileext = TextUtils::RemoveWhitespace(fileext);


	if (filename.empty())
	{

	}

	if (fileext.empty())
	{
		if (!TextUtils::endsWith(filename, L'.'))
		{
			filename += '.';
		}
		filename += _xls_ext;
	}
	
	return paths_ConcatPath(directory, filename)
	*/
}

std::wstring ExcelExport::GetFileExtension() const
{
	return _xls_ext;
}


void ExcelExport::Export(const std::wstring& xlsFile, const std::vector<IInDoc*> & inDocs)
{
	libxl::Book* book = xlCreateBook();
	SetupLicence(book);
	CowPresenceViewCreator viewCreator(m_session);

	BOOST_FOREACH(IInDoc* inDoc, inDocs)
	{
		libxl::Sheet* sheet = book->addSheet(DocNoText(inDoc).ToString().c_str());

		HerdRegistry* registry = m_session->GetHerdRegistry(inDoc->GetHerdId());

		std::vector<ICowPresence*> cows;
		PtrEnumerator<ICowEntry> cowsEnum = inDoc->EnumCows();
		while (cowsEnum.hasNext())
		{
			ICowEntry* entry = *cowsEnum;
			ICowPresence* presence = registry->GetCowPresence(CowPresenceKey(entry->GetCowId(), inDoc->GetDocType(), inDoc->GetId()));
			cows.push_back(presence);
			cowsEnum.advance();
		}
		WriteCows(sheet, cows);
	}

	book->save(xlsFile.c_str());
	book->release();
}

void ExcelExport::SetupLicence(libxl::Book* book)
{
	book->setKey(L"name", L"key");
}

void ExcelExport::Export(const std::wstring& xlsFile, const std::vector<IBuyDoc*> & buyDocs)
{
	libxl::Book* book = xlCreateBook();
	SetupLicence(book);
	CowPresenceViewCreator viewCreator(m_session);

	BOOST_FOREACH(IBuyDoc* buyDoc, buyDocs)
	{
		libxl::Sheet* sheet = book->addSheet(DocNoText(buyDoc).ToString().c_str());

		HerdRegistry* registry = m_session->GetHerdRegistry(buyDoc->GetHerdId());

		std::vector<ICowPresence*> cows;
		PtrEnumerator<ICowEntry> cowsEnum = buyDoc->EnumCows();
		while (cowsEnum.hasNext())
		{
			ICowEntry* entry = *cowsEnum;
			ICowPresence* presence = registry->GetCowPresence(CowPresenceKey(entry->GetCowId(), buyDoc->GetDocType(), buyDoc->GetId()));
			cows.push_back(presence);
			cowsEnum.advance();
		}
		WriteCows(sheet, cows);
	}

	book->save(xlsFile.c_str());
	book->release();
}

void ExcelExport::Export(const std::wstring& xlsFile, uint32_t viewHerdId, const std::vector<IMoveDoc*> & moveDocs)
{
	libxl::Book* book = xlCreateBook();
	SetupLicence(book);
	CowPresenceViewCreator viewCreator(m_session);

	BOOST_FOREACH(IMoveDoc* moveDoc, moveDocs)
	{
		libxl::Sheet* sheet = book->addSheet(DocNoText(moveDoc).ToString().c_str());
		std::vector<ICowPresence*> cows;
		if (viewHerdId == moveDoc->GetSrcHerdId())
		{	
			HerdRegistry* registry = m_session->GetHerdRegistry(viewHerdId);
			PtrEnumerator<ICowEntry> cowsEnum = moveDoc->EnumCows();
			while (cowsEnum.hasNext())
			{
				ICowEntry* entry = *cowsEnum;

				ICowTrail* trail = m_session->GetCowTrail(entry->GetCowId());
				CowPath path = trail->GetPath();
				/*IHerdFootprint* footprint = path.GetFootprint(moveDoc);
				IDoc* entryDoc = footprint->PrevStep()->GetStepDoc();*/
				ICowPresence* presence = registry->GetCowPresence(CowPresenceKey(entry->GetCowId(), moveDoc->GetDocType(), moveDoc->GetId()));
				cows.push_back(presence);
				cowsEnum.advance();
			}


		}
		else if (viewHerdId == moveDoc->GetDstHerdId())
		{
			HerdRegistry* registry = m_session->GetHerdRegistry(viewHerdId);
			PtrEnumerator<ICowEntry> cowsEnum = moveDoc->EnumCows();
			while (cowsEnum.hasNext())
			{
				ICowEntry* entry = *cowsEnum;

				ICowTrail* trail = m_session->GetCowTrail(entry->GetCowId());
				CowPath path = trail->GetPath();
				IHerdFootprint* footprint = path.GetFootprint(moveDoc);
				IDoc* entryDoc = footprint->PrevStep()->GetStepDoc();
				ICowPresence* presence = registry->GetCowPresence(CowPresenceKey(entry->GetCowId(), moveDoc->GetDocType(), moveDoc->GetId()));
				cows.push_back(presence);
				cowsEnum.advance();
			}
		}

		WriteCows(sheet, cows);
	}

	book->save(xlsFile.c_str());
	book->release();
}

void ExcelExport::Export(const std::wstring& xlsFile, const std::vector<IOutDoc*> & outDocs)
{
	libxl::Book* book = xlCreateBook();
	SetupLicence(book);
	CowPresenceViewCreator viewCreator(m_session);

	BOOST_FOREACH(IOutDoc* outDoc, outDocs)
	{
		libxl::Sheet* sheet = book->addSheet(DocNoText(outDoc).ToString().c_str());

		HerdRegistry* registry = m_session->GetHerdRegistry(outDoc->GetHerdId());

		std::vector<ICowPresence*> cows;
		PtrEnumerator<ICowEntry> cowsEnum = outDoc->EnumCows();
		while (cowsEnum.hasNext())
		{
			ICowEntry* entry = *cowsEnum;

			ICowTrail* trail = m_session->GetCowTrail(entry->GetCowId());
			CowPath path = trail->GetPath();
			IHerdFootprint* footprint = path.GetLastStep();
			IDoc* entryDoc = footprint->PrevStep()->GetStepDoc();
			ICowPresence* presence = registry->GetCowPresence(CowPresenceKey(entry->GetCowId(), entryDoc->GetDocType(), entryDoc->GetId()));
			cows.push_back(presence);
			cowsEnum.advance();
		}
		WriteCows(sheet, cows);
	}

	book->save(xlsFile.c_str());
	book->release();
}

void ExcelExport::Export(const std::wstring& xlsFile, const std::vector<ISellDoc*>& sellDocs)
{
	libxl::Book* book = xlCreateBook();
	SetupLicence(book);
	CowPresenceViewCreator viewCreator(m_session);
	
	BOOST_FOREACH(ISellDoc* sellDoc, sellDocs)
	{
		libxl::Sheet* sheet = book->addSheet(DocNoText(sellDoc).ToString().c_str());
		
		HerdRegistry* registry = m_session->GetHerdRegistry(sellDoc->GetHerdId());
		
		std::vector<ICowPresence*> cows;
		PtrEnumerator<ICowEntry> cowsEnum = sellDoc->EnumCows();
		while (cowsEnum.hasNext())
		{
			ICowEntry* entry = *cowsEnum;

			ICowTrail* trail = m_session->GetCowTrail(entry->GetCowId());
			CowPath path = trail->GetPath();
			IHerdFootprint* footprint =  path.GetLastStep();
			IDoc* entryDoc = footprint->PrevStep()->GetStepDoc();
			ICowPresence* presence = registry->GetCowPresence(CowPresenceKey(entry->GetCowId(), entryDoc->GetDocType(), entryDoc->GetId()));
			cows.push_back(presence);
			cowsEnum.advance();
		}
		WriteCows(sheet, cows);
	}

	book->save(xlsFile.c_str());
	book->release();
}


void ExcelExport::Export(const std::wstring& xlsFile, const std::vector<IInvoice*>& invoices)
{
	libxl::Book* book = xlCreateBook();
	SetupLicence(book);
	CowPresenceViewCreator viewCreator(m_session);
	libxl::Sheet* sheet = book->addSheet(StringLoader(IDS_DEFAULTSHEETNAME).c_str());

	
	int row = 1;
	BOOST_FOREACH(IInvoice* invoice, invoices)
	{
		InvoiceViewCreator viewCreator(m_session);
		InvoiceViewText invoiceText;
	
		InvoiceView* invoiceView = viewCreator.CreateView(invoice);
		
		for(int field = 0,count = m_settings->GetFieldCount();field < count;field++)
		{				
			sheet->writeStr(row, field,invoiceText.Text(invoiceView,m_settings->GetField(field)).c_str());
		}

		delete invoiceView;				
		row++;		
	}

	book->save(xlsFile.c_str());
	book->release();
}



void ExcelExport::Export(const std::wstring& xlsFile,const std::vector<ICowPresence*> & cows)
{	
	libxl::Book* book = xlCreateBook();
	SetupLicence(book);
	
	libxl::Sheet* sheet = book->addSheet(StringLoader(IDS_DEFAULTSHEETNAME).c_str());
	WriteCows(sheet,cows);	

	book->save(xlsFile.c_str());
	book->release();	
}


void ExcelExport::WriteCows(libxl::Sheet* pSheet, const std::vector<ICowPresence*> & cows)
{
	CowPresenceViewCreator viewCreator(m_session);
	CowPresenceViewText cowText;
	CowPresenceListHeader header;
	
	int row = 1;
	for (int field = 0, count = m_settings->GetFieldCount(); field < count; field++)
	{
		uint32_t columnId = m_settings->GetField(field);
		const HeaderColumn* column = header.GetColumnWithId(columnId);
		std::wstring columnName = L"";
		if (column != NULL)
			columnName = column->GetColText();
		else
			columnName = IntString(columnId);

		pSheet->writeStr(row, field, columnName.c_str());
	}

	row++;

	auto cowIt = cows.begin();
	
	while(cowIt != cows.end())
	{
		CowPresenceView* presenceView = viewCreator.CreateView(*cowIt);
	
		for(int field = 0,count = m_settings->GetFieldCount();field < count;field++)
		{				
			pSheet->writeStr(row, field,cowText.Text(presenceView, m_settings->GetField(field)).c_str());
		}

		delete presenceView;
		row++;
		cowIt++;
	}
}

void ExcelExport::WriteDocs(libxl::Sheet* pSheet, const std::vector<IOutDoc*> & ownDocs)
{
	OwnDocViewCreator viewCreator(m_session);

	OwnDocViewText text;
	OwnDocsListHeader header;

	int row = 1;
	for (int field = 0, count = m_settings->GetFieldCount(); field < count; field++)
	{
		uint32_t columnId = m_settings->GetField(field);
		const HeaderColumn* column = header.GetColumnWithId(columnId);
		std::wstring columnName = L"";
		if (column != NULL)
			columnName = column->GetColText();
		else
			columnName = IntString(columnId);

		pSheet->writeStr(row, field, columnName.c_str());
	}

	row++;

	auto docIt = ownDocs.begin();

	while (docIt != ownDocs.end())
	{
		OwnDocView* view = viewCreator.CreateView(*docIt);

		for (int field = 0, count = m_settings->GetFieldCount(); field < count; field++)
		{
			pSheet->writeStr(row, field, text.Text(view, m_settings->GetField(field)).c_str());
		}

		delete view;
		row++;
		docIt++;
	}
}

void ExcelExport::WriteDocs(libxl::Sheet* pSheet, const std::vector<IInDoc*> & inDocs)
{
	OwnDocViewCreator viewCreator(m_session);

	OwnDocViewText text;
	OwnDocsListHeader header;

	int row = 1;
	for (int field = 0, count = m_settings->GetFieldCount(); field < count; field++)
	{
		uint32_t columnId = m_settings->GetField(field);
		const HeaderColumn* column = header.GetColumnWithId(columnId);
		std::wstring columnName = L"";
		if (column != NULL)
			columnName = column->GetColText();
		else
			columnName = IntString(columnId);

		pSheet->writeStr(row, field, columnName.c_str());
	}

	row++;

	auto docIt = inDocs.begin();

	while (docIt != inDocs.end())
	{
		OwnDocView* view = viewCreator.CreateView(*docIt);

		for (int field = 0, count = m_settings->GetFieldCount(); field < count; field++)
		{
			pSheet->writeStr(row, field, text.Text(view, m_settings->GetField(field)).c_str());
		}

		delete view;
		row++;
		docIt++;
	}
}

void ExcelExport::WriteDocs(libxl::Sheet* pSheet, const std::vector<IBuyDoc*> & docs)
{
	HentsDocViewCreator viewCreator(m_session);

	HentsDocViewText text;
	HentsDocsListHeader header;

	int row = 1;
	for (int field = 0, count = m_settings->GetFieldCount(); field < count; field++)
	{
		uint32_t columnId = m_settings->GetField(field);
		const HeaderColumn* column = header.GetColumnWithId(columnId);
		std::wstring columnName = L"";
		if (column != NULL)
			columnName = column->GetColText();
		else
			columnName = IntString(columnId);

		pSheet->writeStr(row, field, columnName.c_str());
	}

	row++;

	auto docIt = docs.begin();

	while (docIt != docs.end())
	{
		HentsDocView* view = viewCreator.CreateView(*docIt);

		for (int field = 0, count = m_settings->GetFieldCount(); field < count; field++)
		{
			pSheet->writeStr(row, field, text.Text(view, m_settings->GetField(field)).c_str());
		}

		delete view;
		row++;
		docIt++;
	}
}

void ExcelExport::WriteDocs(libxl::Sheet* pSheet, const std::vector<IMoveDoc*> & docs)
{
	MoveDocViewCreator viewCreator(m_session);

	MoveDocViewText text;
	MoveDocsListHeader header;

	int row = 1;
	for (int field = 0, count = m_settings->GetFieldCount(); field < count; field++)
	{
		uint32_t columnId = m_settings->GetField(field);
		const HeaderColumn* column = header.GetColumnWithId(columnId);
		std::wstring columnName = L"";
		if (column != NULL)
			columnName = column->GetColText();
		else
			columnName = IntString(columnId);

		pSheet->writeStr(row, field, columnName.c_str());
	}

	row++;

	auto docIt = docs.begin();

	while (docIt != docs.end())
	{
		MoveDocView* view = viewCreator.CreateView(*docIt);

		for (int field = 0, count = m_settings->GetFieldCount(); field < count; field++)
		{
			pSheet->writeStr(row, field, text.Text(view, m_settings->GetField(field)).c_str());
		}

		delete view;
		row++;
		docIt++;
	}
}

void ExcelExport::WriteDocs(libxl::Sheet* pSheet, const std::vector<ISellDoc*>& docs)
{
	HentsDocViewCreator viewCreator(m_session);

	HentsDocViewText text;
	HentsDocsListHeader header;

	int row = 1;
	for (int field = 0, count = m_settings->GetFieldCount(); field < count; field++)
	{
		uint32_t columnId = m_settings->GetField(field);
		const HeaderColumn* column = header.GetColumnWithId(columnId);
		std::wstring columnName = L"";
		if (column != NULL)
			columnName = column->GetColText();
		else
			columnName = IntString(columnId);

		pSheet->writeStr(row, field, columnName.c_str());
	}

	row++;

	auto docIt = docs.begin();

	while (docIt != docs.end())
	{
		HentsDocView* view = viewCreator.CreateView(*docIt);

		for (int field = 0, count = m_settings->GetFieldCount(); field < count; field++)
		{
			pSheet->writeStr(row, field, text.Text(view, m_settings->GetField(field)).c_str());
		}

		delete view;
		row++;
		docIt++;
	}
}


void ExcelExport::ExportDocs(const std::wstring& xlsFile, const std::vector<IOutDoc*> & ownDocs)
{
	libxl::Book* book = xlCreateBook();
	SetupLicence(book);

	libxl::Sheet* sheet = book->addSheet(StringLoader(IDS_DEFAULTSHEETNAME).c_str());
	WriteDocs(sheet, ownDocs);

	book->save(xlsFile.c_str());
	book->release();
}

void ExcelExport::ExportDocs(const std::wstring& xlsFile, const std::vector<IInDoc*> & inDocs)
{
	libxl::Book* book = xlCreateBook();
	SetupLicence(book);

	libxl::Sheet* sheet = book->addSheet(StringLoader(IDS_DEFAULTSHEETNAME).c_str());
	WriteDocs(sheet, inDocs);

	book->save(xlsFile.c_str());
	book->release();
}

void ExcelExport::ExportDocs(const std::wstring& xlsFile, const std::vector<IBuyDoc*> & buyDocs)
{
	libxl::Book* book = xlCreateBook();
	SetupLicence(book);

	libxl::Sheet* sheet = book->addSheet(StringLoader(IDS_DEFAULTSHEETNAME).c_str());
	WriteDocs(sheet, buyDocs);

	book->save(xlsFile.c_str());
	book->release();
}

void ExcelExport::ExportDocs(const std::wstring& xlsFile, const std::vector<IMoveDoc*> & moveDocs)
{
	libxl::Book* book = xlCreateBook();
	SetupLicence(book);

	libxl::Sheet* sheet = book->addSheet(StringLoader(IDS_DEFAULTSHEETNAME).c_str());
	WriteDocs(sheet, moveDocs);

	book->save(xlsFile.c_str());
	book->release();

}

void ExcelExport::ExportDocs(const std::wstring& xlsFile, const std::vector<ISellDoc*>& sellDocs)
{
	libxl::Book* book = xlCreateBook();
	SetupLicence(book);

	libxl::Sheet* sheet = book->addSheet(StringLoader(IDS_DEFAULTSHEETNAME).c_str());
	WriteDocs(sheet, sellDocs);

	book->save(xlsFile.c_str());
	book->release();
}