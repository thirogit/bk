#include "stdafx.h"
#include "NullDouble.h"
//#include <limits>
//#include <float.h>
#include <cmath>

#define DEFAULT_SEPARATOR L'.'

FloatFormat::FloatFormat() : m_Precision(0), m_Separator(DEFAULT_SEPARATOR)
{
}

FloatFormat::FloatFormat(int precision, wchar_t sperator) : m_Precision(precision), m_Separator(sperator)
{

}

FloatFormat::FloatFormat(int precision) : m_Precision(precision), m_Separator(DEFAULT_SEPARATOR)
{
}

FloatFormat::FloatFormat(const FloatFormat& src)
{
	m_Precision = src.m_Precision;
	m_Separator = src.m_Separator;
}

wchar_t FloatFormat::GetDecimalPoint() const
{
	return m_Separator;
}

int FloatFormat::GetPrecision() const
{
	return m_Precision;
}




///////////////////////////////////////////////////////////////////////////////////////////////////

NullDouble::NullDouble()
{
	MakeNull();
}

NullDouble::NullDouble(double dValue) 
{
	SetDouble(dValue);
}

NullDouble::NullDouble(const NullDouble& copy)
{	
	CopyFrom(copy);
}

void NullDouble::CopyFrom(const NullDouble& copy)
{
	m_dValue = copy.m_dValue;
}

void NullDouble::MakeNull()
{
	m_dValue.reset();
}

bool NullDouble::IsNull() const
{
	return !m_dValue.is_initialized();
}

void NullDouble::SetDouble(double dValue)
{
	m_dValue = dValue;	
}

double  NullDouble::GetDouble(double dIfNull/* = 0.0*/) const
{
	if(IsNull())
		return dIfNull;

	return m_dValue.get();
}


std::wstring NullDouble::ToString(unsigned int precision/* = NULLDOUBLE_DEFAULT_PREC*/) const
{
	int goodPrecision = precision < 2 ? 2 : precision;
	return ToString(FloatFormat( goodPrecision,L'.'));	
}

std::wstring NullDouble::ToString(const FloatFormat& fmt) const
{
	if(IsNull())
	{
		return L"";
	}

	wchar_t fmtBuffer[100];
	cfltcvt(m_dValue.get(), fmtBuffer, fmt.GetPrecision(), fmt.GetDecimalPoint());
	return fmtBuffer;
}

NullDouble NullDouble::FromString(const std::wstring& s)
{
	if (s.empty())
		return NullDouble();

	double d = wcstod(s.c_str(), NULL);

	return NullDouble(d);
}


NullDouble& NullDouble::operator=(const double right)
{
	SetDouble(right);
	return *this;
}

NullDouble& operator +=(NullDouble &left ,NullDouble &right)
{
	if(!right.IsNull() && !left.IsNull()) 
	{
		left.m_dValue = (left.m_dValue.get() + right.m_dValue.get());
	}
	else
	{
		left.MakeNull();
	}
	return left;    
}

NullDouble& operator+=(NullDouble &left ,const double right)
{
	if(!left.IsNull())
	{
		left.m_dValue = (left.m_dValue.get() + right);
	}
	return left;
}

bool operator<(const NullDouble &left ,double right)
{
	if(left.IsNull())
		return false;

	return left.GetDouble() < right;
}

bool operator>(const NullDouble &left ,double right)
{
	if(left.IsNull())
		return false;

	return left.GetDouble() > right;
}

bool operator!=(const NullDouble &left ,const NullDouble &right)
{
	return !(left == right);
}

bool operator==(const NullDouble &left ,const NullDouble &right)
{
	bool l = left.IsNull();
	bool r = right.IsNull();
	
	if(l && r) return true;
	else if(r || l) return false;

	return (left.m_dValue == right.m_dValue);
}


bool operator>(const NullDouble& left,const NullDouble& right)
{
	if(!left.IsNull()) 
	{
		if(right.IsNull()) return true; //nie-null zawsze wiekszy od null
		else return left.m_dValue > right.m_dValue; 
	}
	return false; //nawet jezeli right nie jest null to left=null > right=nie-null
			     // a gdy oba rowne null to takze zwracamy false bo nie pytamy sie o rownosc
}


bool operator<(const NullDouble &left ,const NullDouble &right)
{
	return right > left;
}


bool operator>=(const NullDouble &left ,const NullDouble &right)
{
	bool l = left.IsNull();
	bool r = right.IsNull();

	if(l && r) return true;
    if(r) return true;
	if(l) return false;

	return left.m_dValue >= right.m_dValue;
}

bool operator<=(const NullDouble &left ,const NullDouble &right)
{
	return right >= left;
}

bool operator>=(const NullDouble &left ,double right)
{
	if(left.IsNull())
		return false;
	return left.GetDouble() >= right;
}

bool operator<=(const NullDouble &left ,double right)
{
	return right >= left;
}
	
bool operator>=(const double left,const NullDouble &right)
{
	if(right.IsNull())
		return true;
	return left >= right.GetDouble();
}

bool operator!=(const NullDouble &left ,const double right)
{
	if(left.IsNull())
		return true;

	return left.GetDouble() != right;
}

bool operator==(const NullDouble &left ,const double right)
{
	return left != right;
}

NullDouble operator*(const NullDouble &left, const double right)
{
	if(left.IsNull())
		return NullDouble();

	return NullDouble(left.GetDouble()*right);
}


#define CVTBUFSIZE        (80)

void NullDouble::cfltcvt(double value, wchar_t *buffer, int precision, wchar_t decimal_point)
{
	int decpt, sign, pos;
	wchar_t *digits = NULL;
	wchar_t cvtbuf[CVTBUFSIZE];
	int capexp = 0;

	digits = cvt(value, precision, &decpt, &sign, cvtbuf, 0);
	if (sign) *buffer++ = '-';
	if (*digits)
	{
		if (decpt <= 0)
		{
			*buffer++ = '0';
			*buffer++ = decimal_point;
			for (pos = 0; pos < -decpt; pos++) *buffer++ = '0';
			while (*digits) *buffer++ = *digits++;
		}
		else
		{
			pos = 0;
			while (*digits)
			{
				if (pos++ == decpt) *buffer++ = decimal_point;
				*buffer++ = *digits++;
			}
		}
	}
	else
	{
		*buffer++ = '0';
		if (precision > 0)
		{
			*buffer++ = decimal_point;
			for (pos = 0; pos < precision; pos++) *buffer++ = '0';
		}
	}


	*buffer = '\0';
}


wchar_t * NullDouble::cvt(double arg, int ndigits, int *decpt, int *sign, wchar_t *buf, int eflag)
{
	int r2;
	double fi, fj;
	wchar_t *p, *p1;

	if (ndigits < 0) ndigits = 0;
	if (ndigits >= CVTBUFSIZE - 1) ndigits = CVTBUFSIZE - 2;
	r2 = 0;
	*sign = 0;
	p = &buf[0];
	if (arg < 0)
	{
		*sign = 1;
		arg = -arg;
	}
	arg = modf(arg, &fi);
	p1 = &buf[CVTBUFSIZE];

	if (fi != 0)
	{
		p1 = &buf[CVTBUFSIZE];
		while (fi != 0)
		{
			fj = modf(fi / 10, &fi);
			*--p1 = (int)((fj + .03) * 10) + '0';
			r2++;
		}
		while (p1 < &buf[CVTBUFSIZE]) *p++ = *p1++;
	}
	else if (arg > 0)
	{
		while ((fj = arg * 10) < 1)
		{
			arg = fj;
			r2--;
		}
	}
	p1 = &buf[ndigits];
	if (eflag == 0) p1 += r2;
	*decpt = r2;
	if (p1 < &buf[0])
	{
		buf[0] = '\0';
		return buf;
	}
	while (p <= p1 && p < &buf[CVTBUFSIZE])
	{
		arg *= 10;
		arg = modf(arg, &fj);
		*p++ = (int)fj + '0';
	}
	if (p1 >= &buf[CVTBUFSIZE])
	{
		buf[CVTBUFSIZE - 1] = '\0';
		return buf;
	}
	p = p1;
	*p1 += 5;
	while (*p1 > '9')
	{
		*p1 = '0';
		if (p1 > buf)
			++*--p1;
		else
		{
			*p1 = '1';
			(*decpt)++;
			if (eflag == 0)
			{
				if (p > buf) *p = '0';
				p++;
			}
		}
	}
	*p = '\0';
	return buf;
}
