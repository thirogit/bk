#include "stdafx.h"
#include "PassportField.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


PassportField::PassportField(const PassportFieldMetrics& metrics, const PassportFieldDescriptor* pFieldDescriptor) : m_fieldMetrics(metrics)
{
	m_pFieldDescriptor = pFieldDescriptor;
}

PassportField::PassportField(const IPassportField* src)
{
	m_pFieldDescriptor = src->GetDescriptor();
	m_fieldMetrics = src->GetFieldMetrics();
}

PassportField::PassportField(const PassportField &copy) : m_fieldMetrics(copy.m_fieldMetrics)
{
	m_pFieldDescriptor = copy.m_pFieldDescriptor;
}

PassportField& PassportField::operator=(const PassportField &right)
{
	m_fieldMetrics = right.m_fieldMetrics;
	m_pFieldDescriptor = right.m_pFieldDescriptor;
	return *this;
}

const PassportFieldDescriptor* PassportField::GetDescriptor() const
{
	return m_pFieldDescriptor;
}

void PassportField::SetFieldMetrics(const PassportFieldMetrics& fieldMetrics)
{
	m_fieldMetrics = fieldMetrics;
}

PassportField::PassportField(const PassportFieldDescriptor* pFieldDescriptor)
{
	m_pFieldDescriptor = pFieldDescriptor;
}

const PassportFieldMetrics& PassportField::GetFieldMetrics() const
{
	return m_fieldMetrics;
}

void PassportField::SetMetric(FIELD_METRIC metric, int metricValue)
{
	switch (metric)
	{
	case X_METRIC:
		m_fieldMetrics.SetX(metricValue);
		break;
	case Y_METRIC:
		m_fieldMetrics.SetY(metricValue);
		break;
	case H_METRIC:
		m_fieldMetrics.SetH(metricValue);
		break;
	case W_METRIC:
		m_fieldMetrics.SetW(metricValue);
		break;
	}
}