#ifndef __ABBYYSTRING_H__
#define __ABBYYSTRING_H__

#include "BstrWrap.h"

class AbbyyString 
{	
public:
	AbbyyString( const CBstr& other );
	AbbyyString& operator=(const CBstr& right);
	const CBstr& GetString() const;
	operator CBstr() const;
	operator BSTR() const;
private:
	CBstr ToPlainString(const CBstr& str);
	wchar_t TranslateAbbyyCharacter(wchar_t chSpecial);
	
	CBstr m_sPlainString;

};






#endif