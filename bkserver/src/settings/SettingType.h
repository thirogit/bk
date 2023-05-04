#pragma once

#include <types/Nullable.h>
#include <string>
#include <boost/optional.hpp>

enum ValueType
{
	ValueType_Integer = 'I',
	ValueType_String = 'S',
	ValueType_Decimal = 'D',
	ValueType_Boolean = 'B',
	ValueType_UInteger = 'U',
	ValueType_None = '\0'
};

class SettingType : public Nullable
{	
	struct _SettingType
	{
		_SettingType();
		bool m_collection;
		bool m_association;
		ValueType m_propertyType;
		ValueType m_keyType;
	};

public:
	SettingType();
	SettingType(const SettingType& src);
	SettingType& operator=(const SettingType& right);

	virtual void MakeNull();
	virtual bool IsNull() const;
	
	void SetIsCollection(bool bCollection);
	void SetIsAssociation(bool bAssociation);
	void SetPropertyType(ValueType type);
	void SetKeyType(ValueType type);

	bool GetIsCollection() const;
	bool GetIsAssociation() const;
	ValueType GetPropertyType() const;
	ValueType GetKeyType() const;	
	
private:	
	void EnsureValue();
	void MakeStr();
#ifdef _DEBUG
	std::wstring m_str;	
	std::wstring ValueTypeToStr(ValueType type);
#endif

	boost::optional<_SettingType> m_Value;
};