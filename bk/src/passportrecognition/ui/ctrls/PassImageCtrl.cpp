#include "stdafx.h"
#include "PassImageCtrl.h"
#include <arrays\DeepDelete.h>

BEGIN_MESSAGE_MAP(CPassImageCtrl, CFreeImageCtrl)
	ON_WM_PAINT()

END_MESSAGE_MAP()

CPassImageCtrl::~CPassImageCtrl()
{
	DeepDelete(m_fields);
}

void CPassImageCtrl::SetProfile(const IPassportRecognitionProfile* profile)
{
	DeepDelete(m_fields);
	m_fields.clear();

	if (profile)
	{
		PtrEnumerator<IPassportField> enumerator = profile->EnumFields();
		while (enumerator.hasNext())
		{
			Field* field = new Field();
			PassportFieldMetrics metrics = enumerator->GetFieldMetrics();
			field->xy.set(metrics.GetX(), metrics.GetY());
			field->wh.set(metrics.GetW(), metrics.GetH());
			m_fields.push_back(field);
			enumerator.advance();
		}
		m_passSizeMM.set(profile->GetXDimention(), profile->GetYDimention());
	}
	
}
void CPassImageCtrl::OnPaint()
{
	CFreeImageCtrl::OnPaint();
	CClientDC dc(this);

	if(m_image.m_hObject)
	{
		CRect rc;
		GetClientRect(&rc);
		CSize bmpSize = m_image.GetBitmapDimension();
		double imgScale = 1.0;	

		if (!(bmpSize.cx < rc.Width() && bmpSize.cy < rc.Height()))
		{
				imgScale = min(double(rc.Width()) / bmpSize.cx, double(rc.Height()) / bmpSize.cy);
		}

		CBrush brush(RGB(0,128,255));
		std::vector<Field*>::iterator it = m_fields.begin();
		while (it != m_fields.end())
		{
			Field* pField = *(it++);

			double x1Scale = imgScale*((double)pField->xy.x) / m_passSizeMM.cx;
			double y1Scale = imgScale*((double)pField->xy.y) / m_passSizeMM.cy;
			
			double x2Scale = imgScale*((double)pField->xy.x+pField->wh.cx)/m_passSizeMM.cx;
			double y2Scale = imgScale*((double)pField->xy.y+pField->wh.cy)/m_passSizeMM.cy;
			
			dc.SetROP2(R2_MASKPEN );
			dc.SelectObject(&brush);
			int x1 = x1Scale*bmpSize.cx;
			int y1 = y1Scale*bmpSize.cy;
			int x2 = x2Scale*bmpSize.cx;
			int y2 = y2Scale*bmpSize.cy;

			dc.Rectangle(x1,y1,x2,y2);
							
				

		}
	}
}