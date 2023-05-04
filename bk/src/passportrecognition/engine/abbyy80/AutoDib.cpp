#include "stdafx.h"
#include "AutoDib.h"

CAutoDib::CAutoDib( HANDLE hDib /*= NULL*/ )
{
	attach( hDib ); 
}

CAutoDib::CAutoDib( const CAutoDib& other ) 
{ 
	attach( other.m_hDib ); 
}

CAutoDib::~CAutoDib() 
{ 
	release(); 
}
    
bool CAutoDib::IsNull() const 
{ 
	return m_hDib == 0; 
}

CAutoDib& CAutoDib::operator = ( const CAutoDib& other )
{
	if( other.m_hDib != m_hDib ) {
		release();
		attach( other.m_hDib );	
    }
	return *this;
}

CAutoDib& CAutoDib::operator = ( HANDLE hDib )
{
	if( m_hDib != hDib ) {
		release();
		attach( m_hDib );	
    }
	return *this;
}

void CAutoDib::attach( HANDLE hDib )
{	
	m_hDib = hDib;	
}

void CAutoDib::release()
{
	if(m_hDib != NULL)
	{
		::GlobalFree(m_hDib);
	}	
}

CAutoDib::operator HANDLE() const
{
	return m_hDib;
}





