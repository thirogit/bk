#ifndef __DECIMALCELLBUILDER_H__
#define __DECIMALCELLBUILDER_H__


#include <types\NullDecimal.h>
#include "EditCellBuilder.h"

class DecimalCellBuilder
{
	class CDecimalEditCell : public CEditCell
	{
		afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);	
		DECLARE_MESSAGE_MAP()
	};

public:
	typedef std::function<void (const NullDecimal&)> DecimalReceiver;

	DecimalCellBuilder();
	DecimalCellBuilder& precision(int prec);
	DecimalCellBuilder& value(const NullDecimal& decimal);
	DecimalCellBuilder& receiver(DecimalReceiver& receiver);


	template<typename F>
	DecimalCellBuilder& receiver(F& f)
	{
		DecimalReceiver recver = f;
		m_receiver = recver;
		return *this;
	}


	void show(CWnd* pParent,const CRect& rect);
protected:
	void OnText(DecimalReceiver fwdRecv,const std::wstring& text);
private:
	int m_precision;
	NullDecimal m_value;
	DecimalReceiver m_receiver;
};





#endif 
