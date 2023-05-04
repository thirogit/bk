#include "stdafx.h"

#include "FloorPlanCtrl.h"
#include <functional>
#include <arrays/DeepDelete.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CONTROL_GAP 5
#define CANVAS_MARGIN 5
#define PIXELS_PER_MILIMETER 2

///////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCanvas, CStatic)
	
	ON_WM_PAINT()
	
END_MESSAGE_MAP()

BOOL CCanvas::Create(DWORD dwStyle,const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CStatic::Create(L"",dwStyle,rect,pParentWnd,nID);
}
void CCanvas::OnPaint()
{
	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);
	
	CBrush white;
	white.Attach(::GetStockObject(WHITE_BRUSH));
	dc.FillRect(&rect,&white);
	white.Detach();

	if(!m_Painter)
	{
		m_Painter(&dc,rect);
	}


}

void CCanvas::SetCanvasPainter(CanvasPainter& painter)
{
	m_Painter = painter;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

#define IDC_CANVAS 1001
#define IDC_TOPRULER 1002
#define IDC_LEFTRULER 1003


FloorPlanCtrl::FloorPlanCtrl()
{
	// Create a hatched bit pattern.
	WORD HatchBits[8] = { 0xAA, 0x55, 0xAA, 0x55,0xAA, 0x55,0xAA, 0x55 };

	CBitmap bm;
	bm.CreateBitmap(8,8,1,1, HatchBits);

	m_TileBrush.CreatePatternBrush(&bm);
}

FloorPlanCtrl::~FloorPlanCtrl()
{
	DeepDelete(m_Tiles);
}

void FloorPlanCtrl::PreSubclassWindow()
{
	ModifyStyleEx(GetExStyle(),WS_EX_CONTROLPARENT);
	ModifyStyle(GetStyle() & ~WS_VISIBLE, WS_CHILD | WS_TABSTOP | WS_GROUP );
	CreateLayout();
	SetWindowText(L"");
}

void FloorPlanCtrl::SetFloorSize(const FloatSize& floorSize)
{
	m_FloorSize = floorSize;
}

Tile* FloorPlanCtrl::AddTile(const std::wstring& sKey,const FloatPoint& tileOrd,const FloatSize& tileSize)
{
	Tile* pOldTile = NULL;

	if(sKey.length() == 0)
		return NULL;

	
	pOldTile = FindTile(sKey);
	

	Tile* pNewTile = new Tile(sKey);
	pNewTile->SetSize(tileSize);
	pNewTile->SetOrd(tileOrd);

	m_Tiles[sKey] = pNewTile;

	if(pOldTile)
		delete pOldTile;

	return pNewTile;

}


Tile* FloorPlanCtrl::FindTile(const std::wstring& sTileKey)
{
	auto findIt = m_Tiles.find(sTileKey);
	if(findIt != m_Tiles.end())
	{
		 return findIt->second;
	}
	return NULL;
}

void FloorPlanCtrl::CreateLayout()
{
    using namespace std::placeholders;

	const int rulerWidth = 28;
	const int rulerOffset = CONTROL_GAP+CANVAS_MARGIN;

	CRect rect;
	GetClientRect(&rect);
	
	CRect topRulerRect(rulerWidth,0,rect.right,rulerWidth);
	m_TopRuler.Create(WS_VISIBLE | WS_CHILD,topRulerRect,this,IDC_TOPRULER);
	m_TopRuler.SetStyle( RWSTYLE_HORZ | RWSTYLE_BOTTOMALIGN );
	m_TopRuler.SetMilimeterPixel( PIXELS_PER_MILIMETER );
	m_TopRuler.SetStartSeperateSize(rulerOffset);

	CRect leftRulerRect(0,rulerWidth,rulerWidth,rect.bottom);
	m_LeftRuler.Create(WS_VISIBLE | WS_CHILD,leftRulerRect,this,IDC_LEFTRULER);
	m_LeftRuler.SetStyle( RWSTYLE_VERT | RWSTYLE_RIGHTALIGN | RWSTYLE_DRAWMODAL );
	m_LeftRuler.SetMilimeterPixel( PIXELS_PER_MILIMETER );
	m_LeftRuler.SetStartSeperateSize(rulerOffset);

	CRect canvasRect(rulerWidth + CONTROL_GAP,rulerWidth + CONTROL_GAP,rect.right,rect.bottom);
	m_Canvas.Create(WS_VISIBLE | WS_CHILD,canvasRect,this,IDC_CANVAS);

	CanvasPainter painter = std::bind(&FloorPlanCtrl::FloorPainter,this,_1,_2);
	m_Canvas.SetCanvasPainter(painter);
}

void FloorPlanCtrl::FloorPainter(CDC* pDC,const CRect& canvasRect)
{
	CRect topRulerBorder = m_TopRuler.GetBorderSize();
	CRect leftRulerBorder = m_LeftRuler.GetBorderSize();

	int orgX = CANVAS_MARGIN+topRulerBorder.left;
	int orgY = CANVAS_MARGIN+leftRulerBorder.top;

	pDC->OffsetViewportOrg(orgX,orgY);

	if(m_FloorSize.cx > 0.0 && m_FloorSize.cy > 0)
	{
		pDC->Rectangle(0,0,PIXELS_PER_MILIMETER * m_FloorSize.cx,PIXELS_PER_MILIMETER * m_FloorSize.cy);
	}

	Tile* pTile;
	FloatPoint pTL,pRB;
	FloatSize size;
	auto tileIt = m_Tiles.begin();
	while(tileIt != m_Tiles.end())
	{
		pTile = tileIt->second;
		size = pTile->GetSize()*PIXELS_PER_MILIMETER;
		pTL = pTile->GetOrd()*PIXELS_PER_MILIMETER;
		pRB = pTL+size;
		pDC->SelectObject(&m_TileBrush);
		pDC->Rectangle(pTL.x,pTL.y,pRB.x,pRB.y);
		tileIt++;
	}

}

void FloorPlanCtrl::Redraw()
{
	m_Canvas.Invalidate();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
Tile::Tile(const std::wstring& sTileKey) : 
		m_sKey(sTileKey),m_pUserData(NULL),
			m_Color(RGB(255,255,255)),m_Ord(0,0),m_Size(0,0)
{
}


		
const std::wstring& Tile::GetKey() const
{
	return m_sKey;
}

const FloatSize& Tile::GetSize() const
{
	return m_Size;
}

const FloatPoint& Tile::GetOrd() const
{
	return m_Ord;
}

void* Tile::GetUserData() const
{
	return m_pUserData;
}

COLORREF Tile::GetColor() const
{
	return m_Color;
}
				
void Tile::SetSize(const FloatSize& size)
{
	m_Size = size;
}

void Tile::SetUserData(void* pData)
{
	m_pUserData = pData;
}

void Tile::SetColor(COLORREF color)
{
}

void Tile::SetOrd(const FloatPoint& ord)
{
	m_Ord = ord;
}
