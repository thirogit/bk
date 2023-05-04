#ifndef __HENTSDOCVIEW_H__
#define __HENTSDOCVIEW_H__

#include "DocView.h"

struct HentsDocViewMember : public DocViewMember
{
	enum 
	{
		Herd = DocViewMember_LastMember,
		Hent,
		Invoice,	
		HentsDocViewMember_LastMember
	};
};

class HentsDocView : public DocView
{
public:
	HentsDocView(uint32_t docId);
	
	const std::wstring&		GetHerdCd() const;
	void SetHerdCd(const std::wstring& herdCd);	
	
	const std::wstring&		GetHentAlias() const;
	void SetHentAlias(const std::wstring& sHentAlias);

	const std::wstring&		GetInvoiceNo() const;
	void SetInvoiceNo(const std::wstring& sInvoiceNo);
		
private:
	std::wstring m_HerdCd;	
	std::wstring m_HentAlias;
	std::wstring m_InvoiceNo;
};

#endif