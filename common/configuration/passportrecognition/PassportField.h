#ifndef __PASSPORTFIELD_H__
#define __PASSPORTFIELD_H__

#include "IPassportField.h"

class PassportField : public IPassportField
{
public:
	PassportField(const IPassportField* src);
	PassportField(const PassportFieldDescriptor* pFieldDescriptor);
	PassportField(const PassportFieldMetrics& metrics,const PassportFieldDescriptor* pFieldDescriptor);
	PassportField(const PassportField &copy);
	PassportField& operator=(const PassportField &right);

	void SetMetric(FIELD_METRIC metric, int metricValue);
	void SetFieldMetrics(const PassportFieldMetrics& fieldMetrics);
	virtual const PassportFieldMetrics& GetFieldMetrics() const;
	virtual const PassportFieldDescriptor* GetDescriptor() const;

public:
	PassportFieldMetrics m_fieldMetrics;
	const PassportFieldDescriptor* m_pFieldDescriptor;

	
};


#endif 