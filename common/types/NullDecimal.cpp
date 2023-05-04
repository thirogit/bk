#include "stdafx.h"
#include "NullDecimal.h"

NullDecimal::NullDecimal() 
{
	
}      

NullDecimal::NullDecimal(const NullDecimal& copy)
{
	m_Value = copy.m_Value;
}

NullDecimal::NullDecimal(const Decimal& src)
{
	m_Value = src;
}

NullDecimal::NullDecimal(long value)
{
	m_Value = Decimal(value);
}

void NullDecimal::MakeNull()
{
	m_Value.reset();
}

bool NullDecimal::IsNull() const
{
	return !m_Value.is_initialized();
}

Decimal NullDecimal::ToDecimal(const Decimal& valueIfNull) const
{
	if(IsNull())
		return valueIfNull;

	return m_Value.get();
}

//
//short NullDecimal::MoneyCents() const   
//{
//	if(IsNull())
//		return 0;
//
//	double intPart; 
//	double fraction;
//
//	Split(intPart,fraction);
//
//	return short(fraction * 100);
//}
//
//long NullDecimal::MoneyInt() const
//{
//	if(IsNull())
//		return ZEROMONEY;
//
//	double intPart; 
//	double fraction;
//	Split(intPart,fraction);
//	return intPart;
//
//}

//CString NullDecimal::ToString(LPCSTR ifNull,const FloatFormat& fmt) const
//{
//	return NullDouble::ToString(ifNull,fmt);
//}
//
//CString NullDecimal::ToString(LPCSTR ifNull/* = NULL*/,unsigned int precision/* = MONEY_DEFAULT_PREC*/) const
//{
//	return NullDouble::ToString(ifNull,precision);
//}

std::wstring NullDecimal::ToString(int prec) const
{
	if(IsNull())
		return L"";

	return m_Value->ToString(prec);
}

std::wstring NullDecimal::ToString(const DecimalFormat& format) const
{
	if (IsNull())
		return L"";

	return m_Value->ToString(format);
}


NullDecimal& NullDecimal::operator+= (const NullDecimal& rs) 
{
	if(IsNull() || rs.IsNull())
		MakeNull();
	else
		m_Value = (m_Value.get() + rs.m_Value.get());

	return *this;
}
NullDecimal& NullDecimal::operator-= (const NullDecimal& rs) 
{
	if(IsNull() || rs.IsNull())
		MakeNull();
	else
		m_Value = (m_Value.get() - rs.m_Value.get());
	return *this;
}

NullDecimal  NullDecimal::operator- ()   const   
{
	NullDecimal  result;  
	if(!IsNull())
		result.m_Value = -(m_Value.get());  
	
	return result;
}

NullDecimal  NullDecimal::operator+ ()   const              
{
	return *this; 
}

NullDecimal  NullDecimal::operator+ (const NullDecimal&  rs)        const 
{
	return NullDecimal(*this) += rs;
}
NullDecimal  NullDecimal::operator- (const NullDecimal&  rs)        const 
{
	return NullDecimal(*this) -= rs;
}

NullDecimal& NullDecimal::operator= (const Decimal& rs)
{
	m_Value = rs;
	return *this;
}

NullDecimal NullDecimal::FromString(const std::wstring& sDecimal)
{
	NullDecimal result;
	if(!sDecimal.empty())
	{
		result = Decimal::FromString(sDecimal);
	}
	return result;
}

bool operator==(const NullDecimal &left ,const NullDecimal &right)
{
	bool l = left.IsNull();
	bool r = right.IsNull();
	
	if(l && r) return true;
	else if(r || l) return false;

	return (left.m_Value == right.m_Value);
}

bool operator!=(const NullDecimal &left ,const NullDecimal &right)
{
	return !(left == right);
}


bool operator>(const NullDecimal &left ,const NullDecimal &right)
{
	if(!left.IsNull()) 
	{
		if(right.IsNull()) return true; //nie-null zawsze wiekszy od null
		else return left.m_Value.get() > right.m_Value.get(); 
	}
	return false; //nawet jezeli right nie jest null to left=null > right=nie-null
			      // a gdy oba rowne null to takze zwracamy false bo nie pytamy sie o rownosc
}

bool operator<(const NullDecimal &left ,const NullDecimal &right)
{
	return right > left;
}


bool operator>=(const NullDecimal &left ,const NullDecimal &right)
{
	if(left == right)
		return true;

	return left > right;
}

bool operator<=(const NullDecimal &left ,const NullDecimal &right)
{
	if(left == right)
		return true;

	return left < right;
}

NullDecimal operator/(const NullDecimal &left, const NullDecimal& right)
{
	if (left.IsNull() || right.IsNull())
		return NullDecimal();

	return NullDecimal(left.ToDecimal(Decimal(0L)) / right.ToDecimal(Decimal(0L)));

}