#include "stdafx.h"
#include "CountryFlagsImageList.h"
#include "../../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
	
CountryFlagsImageList::CountryFlagsImageList() : m_pFlagImageList(NULL)
{
	struct COUNTRY_FLAG_DESCRIPTOR
	{
		UINT number;		
		int flagIndex;
	} 
	flagDescriptors[] =
	{		
		{442	, 29},
		{208	, 13},
		{372	, 23},
		{32		, 3},
		{528	, 34},
		{46		, 5},
		{250	, 17},
		{276	, 12},
		{826	, 18},
		{380	, 25},
		{246	, 16},
		{752	, 40},
		{724	, 15},
		{300	, 19},
		{196	, 10},
		{705	, 41},
		{620	, 37},
		{370	, 33},
		{203	, 11},
		{348	, 22},
		{703	, 42},
		{233	, 14},
		{616	, 36},
		{440	, 28},
		{428	, 30},
		{643	, 39},
		{756	, 8},
		{792	, 44},
		{578	, 35},
		{804	, 45},
		{642	, 38},
		{398	, 26},
		{191	, 21},
		{112	, 7},
		{891	, 9},
		{100	, 6},
		{352	, 24},
		{31		, 4},
		{807	, 32},
		{239	, 20},
		{51		, 2},
		{20		, 1},
		{438	, 27},
		{492	, 31},
		{674	, 43},
		{0		,  -1}
	};
	
	
	COUNTRY_FLAG_DESCRIPTOR* pCountryFlagDescriptor = flagDescriptors;
	while(pCountryFlagDescriptor->number)
	{
		m_FlagsToCountry[pCountryFlagDescriptor->number] = pCountryFlagDescriptor->flagIndex;
		pCountryFlagDescriptor++;
	}

	
}

void CountryFlagsImageList::LazyInitFlagImageList()
{
	if(!m_pFlagImageList)
	{
		m_pFlagImageList = new CBmpImageList();
		m_pFlagImageList->Create(16,16,ILC_COLORDDB | ILC_MASK,22,10);
		CBitmap bmp;
		bmp.LoadBitmap(IDB_FLAGS);
		m_pFlagImageList->Add(&bmp,RGB(255,0,255));
	}
}

CountryFlagsImageList::~CountryFlagsImageList()
{
	if(m_pFlagImageList)
	{
		m_pFlagImageList->DeleteImageList();
		delete m_pFlagImageList;
	}	

}


HICON CountryFlagsImageList::GetFlagByCountryNumber(uint32_t countryNumber)
{
	LazyInitFlagImageList();

	auto flagIt = m_FlagsToCountry.find(countryNumber);
	if(flagIt != m_FlagsToCountry.end())
	{
		return m_pFlagImageList->ExtractIcon(flagIt->second);		
	}

	return NULL;
	
}


int CountryFlagsImageList::GetCountryFlagIndex(uint32_t countryNumber)
{
	auto flagIt = m_FlagsToCountry.find(countryNumber);
	if(flagIt != m_FlagsToCountry.end())
	{
		return flagIt->second;		
	}
	return -1;
}

HBITMAP CountryFlagsImageList::GetFlagBitmap(uint32_t countryNumber,COLORREF clrBkGrnd)
{
  int flagIndex = GetCountryFlagIndex(countryNumber);
 
  if (flagIndex < 0)
    return NULL;
 
  LazyInitFlagImageList();
  
  return m_pFlagImageList->GetBitmap(flagIndex,clrBkGrnd);
}
