#ifndef __FLOORPLANCTRL_H__
#define __FLOORPLANCTRL_H__

#include <string>
#include "RulerWnd.h"
#include <functional>
#include "../../types/Size.h"
#include "../../types/Point.h"
#include <map>

typedef std::function<void (CDC*,const CRect&)> CanvasPainter;

typedef Size<double> FloatSize;
typedef Point<double> FloatPoint;

class CCanvas : public CStatic
{
public:
	virtual BOOL Create(DWORD dwStyle,const RECT& rect, CWnd* pParentWnd, UINT nID);
	void SetCanvasPainter(CanvasPainter& painter);

protected:
	afx_msg void OnPaint() ;	
	DECLARE_MESSAGE_MAP()
	CanvasPainter m_Painter;
};

class Tile;

typedef std::function<void (Tile*)> TileSelectedListener;

class FloorPlanCtrl : public CStatic
{
public:
	FloorPlanCtrl();
	virtual ~FloorPlanCtrl();

	virtual void PreSubclassWindow();

	void SetFloorSize(const FloatSize& floorSize);
	Tile* AddTile(const std::wstring& sKey,const FloatPoint& tileOrd,const FloatSize& tileSize);	
	Tile* FindTile(const std::wstring& sTileKey);
	void RemoveTile(const std::wstring& sTileKey);
	void Redraw();
	
protected:
	
	void CreateLayout();
	void FloorPainter(CDC* pDC,const CRect& canvasRect);
	
protected:
	CBrush m_TileBrush;

	CRulerWnd	m_TopRuler;
	CRulerWnd	m_LeftRuler;
	CCanvas		m_Canvas;
	FloatSize	m_FloorSize;
	std::map<std::wstring,Tile*> m_Tiles;
	
	//DECLARE_MESSAGE_MAP()
};

class Tile
{
	Tile(const std::wstring& sTileKey);
public:
		
	const std::wstring& GetKey() const;
	const FloatSize& GetSize() const;
	void* GetUserData() const;
	COLORREF GetColor() const;
	const FloatPoint& GetOrd() const;
				
	void SetSize(const FloatSize& size);
	void SetUserData(void* pData);
	void SetColor(COLORREF color);
	void SetOrd(const FloatPoint& ord);
private:
	void* m_pUserData;
	COLORREF m_Color;
	FloatSize m_Size;
	FloatPoint m_Ord;
	std::wstring m_sKey;
	

	friend class FloorPlanCtrl;
};



#endif

