#include "stdafx.h"
#include "Decimal.h"
#include <boost/throw_exception.hpp>

DecimalFormat::DecimalFormat() : m_Precision(4),m_DecimalPoint(L'.')
{
}

DecimalFormat::DecimalFormat(int precision,wchar_t decimalpoint) : m_Precision(precision),m_DecimalPoint(decimalpoint)
{
}

DecimalFormat::DecimalFormat(int precision) : m_Precision(precision),m_DecimalPoint(L'.')
{
}

DecimalFormat::DecimalFormat(const DecimalFormat& src)
{
	m_Precision = src.m_Precision;
	m_DecimalPoint = src.m_DecimalPoint;
}

wchar_t DecimalFormat::GetDecimalPoint() const
{
	return m_DecimalPoint;
}

int DecimalFormat::GetPrecision() const
{
	return m_Precision;
}


//-------------------------------------------------------------------------------------------------

Decimal operator "" _d(unsigned long long number)
{
	return Decimal(number);
}

//-------------------------------------------------------------------------------------------------
Decimal::DecimalContext::DecimalContext()
{
	mpd_init(&m_ctx,MPD_DECIMAL128);
}

Decimal::DecimalContext::operator mpd_context_t*()
{
	return &m_ctx;
}

mpd_context_t* Decimal::DecimalContext::operator->()
{
	return &m_ctx;
}

Decimal::DecimalContext Decimal::m_Context;

Decimal::Decimal(mpd_t* decimal)
{
	m_pDecimal = decimal;
	SetStringValue();
}


void Decimal::SetStringValue()
{
#if _DEBUG
	m_stringValue = ToString(6);
#endif
}

Decimal::Decimal()
{
	Create();
	AssignTo(0L);	
}

std::wstring Decimal::ToString(int prec) const
{
	return ToString(DecimalFormat(prec));
}

std::wstring Decimal::ToString(const DecimalFormat& format) const
{
	std::wstring sDecimal;
	uint32_t status = 0;

	wchar_t decPt[2] = {0};
	decPt[0] = format.GetDecimalPoint();

	mpd_spec_t spec;
	spec.min_width = 0;
	spec.prec = format.GetPrecision();
	spec.type = L'f';
	spec.align = L'>';
	spec.sign = L'-';
	spec.dot = decPt;
	spec.sep = L"";
	spec.grouping = L"";

	mpd_char_t* szFormattedDecimal = mpd_qformat_spec(m_pDecimal, &spec, m_Context, &status);	
	if(szFormattedDecimal)
	{
		sDecimal = szFormattedDecimal;
		mpd_free(szFormattedDecimal);
	}
	return std::wstring(sDecimal.begin(),sDecimal.end());
}

Decimal Decimal::FromString(const std::wstring& sDecimal)
{
	uint32_t status = 0;
	Decimal result;
	mpd_qset_string(result.m_pDecimal,sDecimal.c_str(),result.m_Context,&status);

	if(status & MPD_Conversion_syntax )
		BOOST_THROW_EXCEPTION(decimal_error(L"decimal conversion error"));

	result.SetStringValue();
	return result;

}

void Decimal::Create()
{
	m_pDecimal = mpd_new(m_Context);
}

void Decimal::Destroy()
{
	mpd_del(m_pDecimal);
}

Decimal::Decimal(const Decimal& src)
{	
	m_pDecimal = NULL;
	CopyFrom(src);
}

Decimal::Decimal(long value)
{
	Create();
	AssignTo(value);
}

Decimal::Decimal(unsigned long value)
{
	Create();
	AssignTo(value);
}

Decimal::Decimal(unsigned long long value)
{
	Create();
	uint32_t status = 0;
	mpd_qset_u64(m_pDecimal, value, m_Context, &status);
	SetStringValue();
}



void Decimal::AssignTo(long value)
{
	uint32_t status = 0;
	mpd_qset_i32(m_pDecimal,value,m_Context,&status);
	SetStringValue();
}

void Decimal::AssignTo(unsigned long value)
{
	uint32_t status = 0;
	mpd_qset_u32(m_pDecimal,value,m_Context,&status);
	SetStringValue();
}

void Decimal::AssignTo(uint8_t sign,mpd_ssize_t exp,const mpd_uint_t* value, mpd_ssize_t valuesize)
{	
	uint32_t status = 0;

	if(exp >= m_Context->emin && exp <=  m_Context->emax)
	{
		m_pDecimal->flags = 0;
		m_pDecimal->exp = exp;
		
		m_pDecimal->flags |= (sign & (MPD_POS | MPD_NEG));
				
		if(m_pDecimal->alloc < valuesize)
		{
			mpd_qresize(m_pDecimal,valuesize,&status);
		}
			
		memcpy(m_pDecimal->data,value,valuesize*sizeof(mpd_uint_t));

		m_pDecimal->len = valuesize;
		//while(value[m_pDecimal->len] != 0 && m_pDecimal->len < m_pDecimal->alloc) 
		//	m_pDecimal->len++;
		
		mpd_setdigits(m_pDecimal);
		mpd_qfinalize(m_pDecimal,m_Context,&status);

		SetStringValue();
	}
}

Decimal::~Decimal()
{
	Destroy();
}

Decimal& Decimal::operator=(const Decimal& right)
{
	CopyFrom(right);
	return *this;
}

Decimal& Decimal::operator=(long right)
{
	AssignTo(right);
	return *this;
}

Decimal& Decimal::operator=(unsigned long right)
{
	AssignTo(right);
	return *this;
}

long long Decimal::Integer() const
{
	uint32_t status = 0;
	mpd_t *result = mpd_new(m_Context);
	mpd_qtrunc(result, m_pDecimal, m_Context, &status);
#ifdef CONFIG_64
	return mpd_qget_i64(result, &status);
#else
	return mpd_qget_i32(result, &status);
#endif
}

Decimal Decimal::Fraction() const
{
	uint32_t status = 0;

	mpd_t *floor = mpd_new(m_Context);
	mpd_qfloor(floor, m_pDecimal, m_Context, &status);
	
	Decimal result;
	mpd_qsub(result.m_pDecimal, this->m_pDecimal, floor, m_Context, &status);
	mpd_del(floor);

	return result;
}

Decimal Decimal::Round(int decimalPlaces) const
{
	uint32_t status = 0;

	Decimal shifted;
	mpd_qshiftl(shifted.m_pDecimal,m_pDecimal, decimalPlaces,&status);
	Decimal integer(shifted);
	mpd_qround_to_intx(integer.m_pDecimal, shifted.m_pDecimal, shifted.m_Context,&status);

	mpd_qrescale(shifted.m_pDecimal, integer.m_pDecimal, shifted.m_pDecimal->exp, shifted.m_Context, &status);
		
	Decimal result;
	mpd_qshiftr(result.m_pDecimal, shifted.m_pDecimal, decimalPlaces,&status);
	return result;
}


/*
Decimal& Decimal::operator=(double right)
{
	m_Context.m_ctx.
	return *this;
}
*/
void Decimal::CopyFrom(const Decimal& src)
{
	if(m_pDecimal)
		mpd_del(m_pDecimal);
	uint32_t status = 0;
	m_pDecimal = mpd_new(m_Context);
	mpd_qcopy( m_pDecimal,src.m_pDecimal,&status);
	SetStringValue();
}


int Decimal::CompareTo(const Decimal& reference) const
{
	uint32_t status = 0;
	mpd_t *pResult = mpd_new(m_Context);
	int cmpResult = mpd_qcompare(pResult, this->m_pDecimal, reference.m_pDecimal, m_Context, &status);
	mpd_del(pResult);
	return cmpResult;
}

Decimal& Decimal::operator+=(const Decimal& right)
{
	uint32_t status = 0;
	mpd_t *pResult = mpd_new(m_Context);
	mpd_qadd(pResult, this->m_pDecimal, right.m_pDecimal, m_Context, &status);
	mpd_qcopy(this->m_pDecimal, pResult,  &status);
	mpd_del(pResult);
	SetStringValue();
	return *this;
}

Decimal& Decimal::operator-=(const Decimal& right)
{
	uint32_t status = 0;
	mpd_t *pResult = mpd_new(m_Context);
	mpd_qsub(pResult, this->m_pDecimal, right.m_pDecimal, m_Context, &status);
	mpd_qcopy(this->m_pDecimal, pResult,  &status);
	mpd_del(pResult);
	SetStringValue();
	return *this;
}

Decimal  Decimal::operator- ()   const
{
	uint32_t status = 0;
	mpd_t *pResult = mpd_new(m_Context);
	mpd_qminus(pResult, this->m_pDecimal, m_Context,&status);
	return Decimal(pResult);
}


bool operator==(const Decimal &left ,const Decimal &right)
{
	return left.CompareTo(right) == 0;
}

bool operator!=(const Decimal &left ,const Decimal &right)
{
	return !(left == right);
}

bool operator>(const Decimal &left ,const Decimal &right)
{
	return left.CompareTo(right) > 0;
}

bool operator<(const Decimal &left ,const Decimal &right)
{
	return right > left;
}

bool operator>=(const Decimal &left ,const Decimal &right)
{
	return left.CompareTo(right) >= 0;
}

bool operator<=(const Decimal &left ,const Decimal &right)
{
	return left.CompareTo(right) <= 0;
}

Decimal operator*(const Decimal &left, const Decimal& right)
{
	uint32_t status = 0;
	Decimal result;
	mpd_qmul(result.m_pDecimal,left.m_pDecimal,right.m_pDecimal,result.m_Context,&status);
	result.SetStringValue();
	return result;
}


Decimal operator+(const Decimal &left, const Decimal& right)
{
	uint32_t status = 0;
	Decimal result;
	mpd_qadd(result.m_pDecimal,left.m_pDecimal,right.m_pDecimal,result.m_Context,&status);
	result.SetStringValue();
	return result;
}


Decimal operator/(const Decimal &left, const Decimal& right)
{
	uint32_t status = 0;
	Decimal result;
	mpd_qdiv(result.m_pDecimal,left.m_pDecimal,right.m_pDecimal,result.m_Context,&status);
	result.SetStringValue();
	return result;
}

Decimal operator-(const Decimal &left, const Decimal& right)
{
	uint32_t status = 0;
	Decimal result;
	mpd_qsub(result.m_pDecimal,left.m_pDecimal,right.m_pDecimal,result.m_Context,&status);
	result.SetStringValue();
	return result;
}