#ifndef __MOVEDOCVIEW_H__
#define __MOVEDOCVIEW_H__

#include "DocView.h"

struct MoveDocViewMember : public DocViewMember
{
	enum 
	{
		SrcHerd = DocViewMember_LastMember,
		DstHerd
	};
};

class MoveDocView : public DocView
{
public:
	MoveDocView(uint32_t docId);
	const std::wstring&		GetSrcHerdCd() const;
	void SetSrcHerdCd(const std::wstring& srcHerdCd);	
	const std::wstring&		GetDstHerdCd() const;
	void SetDstHerdCd(const std::wstring& dstHerdCd);	
private:
	std::wstring m_SrcHerdCd;	
	std::wstring m_DstHerdCd;	
};

#endif