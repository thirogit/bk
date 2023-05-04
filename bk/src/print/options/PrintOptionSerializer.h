#ifndef __PRINTOPTIONSERIALIZER_H__
#define __PRINTOPTIONSERIALIZER_H__

#include <stdint.h>
#include <string>
#include <types\Decimal.h>
#include <configuration\print\PrintOption.h>
#include <types\NullInt.h>
#include <types\NullDecimal.h>
#include <types\DateTime.h>

class PrintOptionSerializer
{
public:
	
	std::wstring GetStringValue(const PrintOption& option);
	NullInt GetIntegerValue(const PrintOption& option);
	uint32_t GetDisplacementEventsValue(const PrintOption& option);
	DecimalFormat GetDecimalFormatValue(const PrintOption& option);
	bool GetBooleanValue(const PrintOption& option);
	NullDecimal GetDecimalValue(const PrintOption& option);
	DateTime GetDateTimeValue(const PrintOption& option);
		
	void SetValue(PrintOption& option,const std::wstring& s);
	void SetValue(PrintOption& option,const NullInt& i);
	void SetValue(PrintOption& option,uint32_t uiEvents);
	void SetValue(PrintOption& option,const DecimalFormat& decFmt);
	void SetValue(PrintOption& option,bool b);
	void SetValue(PrintOption& option, const NullDecimal& d);
	void SetValue(PrintOption& option, const DateTime& dt);
	
};



#endif
