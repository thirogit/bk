#ifndef __PASSPORTRECOGNITIONFIELDSCONSTRUCTOR_H__
#define __PASSPORTRECOGNITIONFIELDSCONSTRUCTOR_H__

#include "PassportRecognizer.h"
#include "configuration\passportrecognition\PassportFieldDescriptor.h"
#include "PassportRecognitionResult.h"
#include <boost/assign/list_inserter.hpp>

class PassportRecognitionFieldsConstructor
{	
	typedef void(PassportRecognitionResult::*FieldSetterFunc)(const std::wstring& sValue) ;
	typedef std::map<std::wstring, FieldSetterFunc> FieldSettersMap;

	struct add_to_map
    {
        FieldSettersMap& m;
    
        add_to_map( FieldSettersMap& m ) : m(m) {}
    
		void operator()(const PassportFieldDescriptor* pField, FieldSetterFunc func)
        {
			m[ pField->GetFieldName() ] = func; 
        }

		boost::assign::list_inserter< add_to_map >  put(const PassportFieldDescriptor* pField, FieldSetterFunc func)
		{
			return boost::assign::make_list_inserter( add_to_map( m ) )( pField, func );
		}
    };
	

public:
	PassportRecognitionFieldsConstructor(PassportRecognitionResult* pResult);
	void SetField(const PassportFieldDescriptor* pField, const std::wstring& sValue);

	
	
private:
	PassportRecognitionResult*	m_pResult;
	FieldSettersMap				m_FieldSettersMap;
	
};



#endif 
