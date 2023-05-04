#include "stdafx.h"
#include "SnapshotListContent.h"
#include <arrays/DeepDelete.h>
#include <string\TextUtils.h>

SnapshotListRow::SnapshotListRow(std::unique_ptr<DownloadSnapshotHeader>& snapshot)
{
	m_Content = snapshot.release();
}

SnapshotListRow::~SnapshotListRow()
{
	delete m_Content;
}

std::wstring SnapshotListRow::CellValue(uint32_t colId)
{
	switch (colId)
	{
	case DownloadSnapshotHeaderMember::HerdIndex:
		return TextUtils::Format(L"%03d", m_Content->GetHerdIndex());
	case DownloadSnapshotHeaderMember::DownloadDtTm:
		return m_Content->GetDownloadDtTm().GetDateInFmt(DateTime::YYYYdashMMdashDD_HHcolonMI);
	case DownloadSnapshotHeaderMember::UploadDtTm:
		return m_Content->GetUploadDtTm().GetDateInFmt(DateTime::YYYYdashMMdashDD_HHcolonMI);
	case DownloadSnapshotHeaderMember::User:
		return m_Content->GetUploadUser();
	}

	return L"";
}

Color SnapshotListRow::CellColor(uint32_t colId)
{
	return Color();
}

const DownloadSnapshotHeader* SnapshotListRow::Content() const
{
	return m_Content;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

SnapshotListContent::SnapshotListContent()
{
	
}

SnapshotListContent::~SnapshotListContent()
{
	DestroyContent();
}

void SnapshotListContent::Add(const DownloadSnapshotHeader* snapshot)
{
	m_Content.push_back(new SnapshotListRow(std::unique_ptr<DownloadSnapshotHeader>(new DownloadSnapshotHeader(*snapshot))));
	NotifyCountChanged();
}

SnapshotListRow* SnapshotListContent::FindRow(uint32_t id)
{
	RowsIndex::index<tag_snapshotId>::type& idindex = m_Content.get<tag_snapshotId>();

	auto it = idindex.find(id);
	if (it != idindex.end())
	{
		return *it;
	}
	return NULL;
}

void SnapshotListContent::Remove(uint32_t id)
{
	RowsIndex::index<tag_snapshotId>::type& idindex = m_Content.get<tag_snapshotId>();

	auto it = idindex.find(id);
	if (it != idindex.end())
	{
		delete *it;
		idindex.erase(it);
		NotifyCountChanged();
	}
	
}

void SnapshotListContent::DestroyContent()
{
	DeepDelete(m_Content);
	m_Content.clear();
}
	
int SnapshotListContent::GetRowCount() const
{
	return m_Content.size();
}

ListRow* SnapshotListContent::GetRow(int row)
{
	return m_Content[row];
}

void SnapshotListContent::SortContent(const SortAttributes& attributes)
{
	
}

const DownloadSnapshotHeader* SnapshotListContent::GetSnapshotRowAt(int iIndex)
{
	return m_Content[iIndex]->Content();
}

