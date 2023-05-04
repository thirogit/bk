#include "stdafx.h"
#include "NullInt.h"
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#ifdef MSVC 
	
#define strcpy_safe(dest,size,src) strcpy_s(dest,size,src)

#else

#include <cstring>
#define strcpy_safe(dest,size,src) strncpy(dest,src,size)

#endif

NullInt::NullInt() 
{
}

NullInt::NullInt(int Value) : m_Value(Value)
{
}

void NullInt::CopyFrom(const NullInt& copy)
{	
	m_Value = copy.m_Value;
}

NullInt::NullInt(const NullInt& copy)
{
	CopyFrom(copy);
}

NullInt& NullInt::operator=(const int right)
{
	m_Value = right;
	return *this;
}

NullInt& NullInt::operator=(const NullInt& right)
{
	CopyFrom(right);
	return *this;
}

bool NullInt::IsNull() const
{
	return !m_Value.is_initialized();
}

void NullInt::MakeNull()
{
	m_Value.reset();
}


int NullInt::ToInt(int whatIfNull/* = 0*/) const
{
	return IsNull() ? whatIfNull : m_Value.get();
}

NullInt NullInt::FromString(const std::wstring& s)
{
	NullInt result;
	if (!s.empty())
	{
		result = boost::lexical_cast<int>(s);
	}

	return result;
}

//void NullInt::ToString(char *pszDest,size_t size,const char* whatIfNull/* = NULL*/) const
//{
//	if(IsNull())
//	{
//		if(whatIfNull)
//			strcpy_safe(pszDest,size,whatIfNull);
//		else
//			pszDest[0] = '\0';
//	}
//	else
//	{
//		sprintf_safe(pszDest,size,"%d",m_Value.get());
//	}
//}


std::wstring NullInt::ToString() const
{
	std::wstring ret;
	if(!IsNull())
	{		
		ret = (boost::wformat(L"%d") % m_Value.get()).str();		
	}
	return ret;
}
	
	
bool operator>=(const NullInt& left,const NullInt& right)
{
	bool l = left.IsNull();
	bool r = right.IsNull();

	if(l && r) return true;
    if(r) return true;
	if(l) return false;

	return left.m_Value.get() >= right.m_Value.get();
}

bool operator>(const NullInt& left,const NullInt& right)
{
	if(!left.IsNull()) 
	{
		if(right.IsNull()) return true; //nie-null zawsze wiekszy od null
		else return left.m_Value.get() > right.m_Value.get(); 
	}
	return false; //nawet jezeli right nie jest null to left=null > right=nie-null
			     // a gdy oba rowne null to takze zwracamy false bo nie pytamy sie o rownosc
}

bool operator<(const NullInt& left,const NullInt& right)
{
	return right > left;
}


bool operator<=(const NullInt& left,const NullInt& right)
{
	return right >= left;
}

bool operator<=(const int left,const NullInt& right)
{
	if(right.IsNull()) return false;
	return right.m_Value.get() >= left;
}

bool operator>=( int left, const NullInt& right)
{
	return operator<=(right,left);
}

bool operator>=(const NullInt& left,const int right)
{
	if(left.IsNull()) return false;
	return left.m_Value.get() >= right;
}

bool operator<=(const NullInt& left,const int right)
{
	if(left.IsNull()) return true;
	return  left.m_Value.get() <= right ;
}

bool operator!=(const NullInt &left ,const NullInt &right)
{	
	return !(left == right);
}


bool operator==(const NullInt &left ,const NullInt &right)
{
	bool l = left.IsNull();
	bool r = right.IsNull();
	
	if(l && r) return true;
	else if(r || l) return false;

	return (left.m_Value.get() == right.m_Value.get());
}

NullInt& operator +=(NullInt &left ,NullInt &right)
{
	if(!right.IsNull()) 
	{
		if(!left.IsNull())
		{
			left.m_Value = left.m_Value.get() + right.m_Value.get();
		}
		else 
		{
				left.m_Value = right.m_Value;
		}
	}
	return left;
    
}

bool operator==(const int left ,const NullInt &right)
{
	if(right.IsNull()) return false;

	return (left == right.m_Value.get());
}

bool operator==(const NullInt &left ,const int right)
{
	return operator==(right,left);
}

bool operator>(const NullInt& left,const int right)
{
	if(left.IsNull())
		return false;

	return left.m_Value.get() > right;
}
bool operator!=(const int left ,const NullInt &right)
{
	return !operator==(left,right);
}

bool operator!=(const NullInt &left,const int right)
{
	return !operator==(left,right);
}