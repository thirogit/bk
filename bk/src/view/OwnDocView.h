#ifndef __OWNDOCVIEW_H__
#define __OWNDOCVIEW_H__

#include "DocView.h"


struct OwnDocViewMember : public DocViewMember
{
	enum 
	{
		Herd = DocViewMember_LastMember					
	};
};

class OwnDocView : public DocView
{
public:
	OwnDocView(uint32_t docId);
	const std::wstring&		GetHerdCd() const;
	void SetHerdCd(const std::wstring& herdCd);	
private:
	std::wstring m_HerdCd;	
};

#endif