#ifndef __NUMERICCELLBUILDER_H__
#define __NUMERICCELLBUILDER_H__

#include "../../ctrls/numericedit/NumericEdit.h"
#include <functional>
#include <types\NullDouble.h>
#include <types\NullDecimal.h>


typedef std::function<void(const std::wstring&)> NumberTextReceiver;
typedef std::function<void(const NullDecimal&)> DecimalReceiver;
typedef std::function<void(const NullDouble&)> FloatReceiver;

class NumericCellBuilder
{
	typedef std::function<std::wstring (int precision)> ValueSetter;

	class CNumericCell : public CNumericEdit
	{
	public:		
		CNumericCell(int precision);
		int Create(const RECT &rect,CWnd *pParentWnd,UINT nID);		
	
	protected:
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		void    SendText();		
	private:
		NumberTextReceiver m_txtreceiver;
		DecimalReceiver m_decreceiver;
		FloatReceiver m_fltreceiver;
	protected:
		afx_msg void OnKillFocus(CWnd* pNewWnd);
		afx_msg void OnNcDestroy();
		afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg UINT OnGetDlgCode();	
		DECLARE_MESSAGE_MAP()
		
		friend NumericCellBuilder;
	};

public:
	NumericCellBuilder();
	NumericCellBuilder& precision(int precision);
	NumericCellBuilder& value(const NullDecimal& decvalue);
	NumericCellBuilder& value(const NullDouble& dvalue);
		
	NumericCellBuilder& receiver(NumberTextReceiver& recver);
	NumericCellBuilder& receiver(DecimalReceiver& recver);
	NumericCellBuilder& receiver(FloatReceiver& recver);
	
	void show(CWnd* pParent,const CRect& rect);
protected:
	std::wstring set_decimal(const NullDecimal& value, int precision);
	std::wstring set_double(const NullDouble& value, int precision);
private:
	int m_precision;
	ValueSetter m_setter;
	NumberTextReceiver m_txtrecver;
	DecimalReceiver m_decrecver;
	FloatReceiver m_floatrecver;
};





#endif 
