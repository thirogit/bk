#ifndef __AUTODIB_H__
#define __AUTODIB_H__

class CAutoDib 
{
public:
	
    CAutoDib( HANDLE hDib = NULL );
    CAutoDib( const CAutoDib& other );
	
    ~CAutoDib();
	
	bool IsNull() const;
	
	CAutoDib& operator = ( const CAutoDib& other );
	CAutoDib& operator = ( HANDLE hDib );

	operator HANDLE() const;

private:
	
	void attach( HANDLE hDib );
	void release();

    HANDLE m_hDib;	    
};

#endif