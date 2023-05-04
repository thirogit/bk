// © ABBYY. 2010.
// SAMPLES code is property of ABBYY, exclusive rights are reserved. 
//
// DEVELOPER is allowed to incorporate SAMPLES into his own APPLICATION and modify it under 
// the  terms of  License Agreement between  ABBYY and DEVELOPER.


// ABBYY FineReader Engine 10 Sample

// Helper functions for BSTR handling

#ifndef BstrWrap_h
#define BstrWrap_h

#include <wtypes.h>
#include <string>

// Simple wrapper class for BSTR
class CBstr {
public:
	// Constructors
	CBstr() { string = SysAllocString( L"" ); }
	CBstr(const std::wstring& s) { string = ::SysAllocString(s.c_str()); }
	CBstr( const wchar_t* str ) { string = ::SysAllocString( str ); }
	CBstr( const CBstr& other ) { string = ::SysAllocString( other ); }
	
	// Destructor
	virtual ~CBstr() { freeBuffer(); }
	
	// Length of the string
	int Length() const { return wcslen( string ); }

	// Returns pointer to the internal buffer
	BSTR* GetBuffer() { freeBuffer(); return &string; }

	// Extractors
	operator wchar_t*() const { return string; }
	BSTR* operator &() { return &string; }
	operator std::wstring() const { return std::wstring(string); }

	wchar_t operator[](int index) { return string[index]; }

	// Operators
	CBstr& operator +=( const CBstr& other );
	CBstr operator + ( const CBstr& other );

	// Assignment operators
	CBstr& operator = ( const wchar_t* str );
	CBstr& operator = ( const CBstr& other );
	
	// Comparison operators
	bool operator != ( const CBstr& other ) { return wcscmp( *this, other ) != 0; }
	bool operator == ( const CBstr& other ) { return wcscmp( *this, other ) == 0; }

private:
	// Free string buffer
	void freeBuffer();
	BSTR string;
};

// Concatenates two wide-character strings ( e.g. BSTRs ) and returns result as BSTR. 
BSTR Concatenate( const wchar_t* str1, const wchar_t* str2 );

inline CBstr& CBstr::operator +=( const CBstr& other ) 
{ 
	*this = Concatenate( this->string, other ); 
	return *this; 
}

inline CBstr CBstr::operator + ( const CBstr& other ) 
{ 
	return Concatenate( *this, other ); 
}

inline CBstr operator + ( const CBstr& first, const CBstr& second )
{
	return Concatenate( first, second );
}

inline CBstr& CBstr::operator = ( const wchar_t* str ) 
{ 
	freeBuffer(); 
	string = ::SysAllocString( str ); 
	return *this;
}

inline CBstr& CBstr::operator = ( const CBstr& other ) 
{ 
	freeBuffer(); 
	string = ::SysAllocString( other ); 
	return *this;
}

#endif // BstrWrap_h

