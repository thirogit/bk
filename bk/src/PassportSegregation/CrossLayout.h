#ifndef __CROSSLAYOUT_H__
#define __CROSSLAYOUT_H__

#include <array>

class CrossLayout 
{
	typedef std::array<CWnd*,4> CrossArray;

	enum Fourth : int
	{
		Fourth_NorthWest = 0,
		Fourth_NorthEast = 1,
		Fourth_SouthWest = 2,
		Fourth_SouthEast = 3,
		Fourth_None = -1		
	};

public:
	

public:
	CrossLayout(CPoint origin);
	bool Add(CWnd* pWnd);	
	CSize GetDesirableSize() const;
protected:
	CSize GetFourthSize(Fourth fourth) const;
	CWnd* GetFourth(Fourth fourth) const;
	void PositionFourths();
	CSize GetWindowSize(CWnd* pWnd) const;

private:
	CPoint m_origin;
	CrossArray m_Fourths;

};
#endif