#pragma once

#include "IPurchaseDownloadIssue.h"

class PurchaseDownloadIssueSerializer;

class PurchaseDownloadIssue : public IPurchaseDownloadIssue
{
public:
	PurchaseDownloadIssue(ItemId id);	
	PurchaseDownloadIssue(const IPurchaseDownloadIssue* src);
	PurchaseDownloadIssue(const PurchaseDownloadIssue& src);

	void CopyFrom(const IPurchaseDownloadIssue* src);

	virtual ItemId GetId() const;
	virtual const PurchaseObjPath& GetPath() const;
	virtual const PurchaseDownloadIssueCategory& GetCategory() const;
	virtual const std::wstring& GetError() const;
	virtual uint32_t GetSolutions() const;
	
	void SetPath(const PurchaseObjPath& path);
	void SetCategory(const PurchaseDownloadIssueCategory& category);
	void SetError(const std::wstring& error);
	void SetSolutions(uint32_t solutions);
	
private:
	ItemId m_issueId;
	PurchaseObjPath m_path;
	PurchaseDownloadIssueCategory m_category;
	std::wstring m_errorTokens;
	uint32_t m_Solutions;

	friend class PurchaseDownloadIssueSerializer;
};


