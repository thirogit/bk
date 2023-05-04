#include "stdafx.h"
#include "TypesSerialization.h"
#include "../countries/Countries.h"
#include <SF/string.hpp>

void serialize(SF::Archive &archive,FarmNo& farmNo)
{
	if(archive.isRead())
	{
		bool isnull;
		unsigned int countryIsoNum;
		std::wstring number;
		archive & isnull & countryIsoNum & number;

		if(!isnull)
			farmNo = FarmNoFactory::Create(Countries::GetCountryByNumber(countryIsoNum),number);
		else
			farmNo.MakeNull();
	}
	else
	{
		archive & farmNo.IsNull() & farmNo.GetCountry().GetCountryISONumber() & farmNo.GetNumber();
	}
}

void serialize(SF::Archive &archive,AccountNo& accountNo)
{
	if(archive.isRead())
	{
		bool isnull;
		unsigned int countryIsoNum;
		std::wstring number;

		archive & isnull & countryIsoNum & number;
		if(!isnull)
			accountNo = AccountNoFactory::Create(Countries::GetCountryByNumber(countryIsoNum),number);
		else
			accountNo.MakeNull();
	}
	else
	{
		archive & accountNo.IsNull() & accountNo.GetCountry().GetCountryISONumber() & accountNo.GetNumber();
	}
}

void serialize(SF::Archive &archive,DateTime& dt)
{
	if(archive.isRead())
	{
		bool bNull;
		uint32_t secondsSinceEpoh;

		archive & bNull & secondsSinceEpoh;

		if(bNull)
			dt.MakeNull();
		else
			dt.SetDateTime(secondsSinceEpoh);
	}
	else
	{
		archive & dt.IsNull() & dt.SecondsSinceEpoh();
	}
}

void serialize(SF::Archive &archive,HentNo& hentNo)
{
	bool null;
	if(archive.isWrite())
	{
		null = hentNo.IsNull();
		archive & null;
		FarmNo farmNo(hentNo.GetFarmNo());
		serialize(archive,farmNo);
		archive & hentNo.GetHerdNo().GetIndex();
	}
	else
	{
		archive & null;
		int herdIndex;
		FarmNo farmNo;
		serialize(archive,farmNo);
		archive & herdIndex;

		if(null)
			hentNo.MakeNull();
		else
			hentNo = HentNoFactory::Create(farmNo,HerdNoFactory::Create(herdIndex));
	}
}


void serialize(SF::Archive &archive,NullDecimal& decimal)
{	
	if(archive.isWrite())
	{
		Decimal nullValue = decimal.ToDecimal(Decimal(0L));
		archive & decimal.IsNull();
		archive & nullValue;
	}
	else
	{
		Decimal value;
		bool null;
		archive & null;
		archive & value;

		if(null)
			decimal.MakeNull();
		else
			decimal = value;
	}
}

void serialize(SF::Archive &archive,Decimal& decimal)
{
	/*
	class DecimalSerializer : public Decimal
	{
	public:
		DecimalSerializer(Decimal& target) : Decimal(target),m_target(target) {}

		void serialize(SF::Archive &archive)
		{			
			if(archive.isWrite())
			{		
				archive & m_pDecimal->flags; 
				archive & m_pDecimal->exp;     
				archive & m_pDecimal->digits;  
				archive & m_pDecimal->len;     
				archive & m_pDecimal->alloc;

				for(int i = 0;i < m_pDecimal->alloc;i++)
					archive & m_pDecimal->data[i];			

			}
			else
			{
				archive & m_pDecimal->flags; 
				archive & m_pDecimal->exp;     
				archive & m_pDecimal->digits;  
				archive & m_pDecimal->len;     
				archive & m_pDecimal->alloc;  				
				for(int i = 0;i < m_pDecimal->alloc;i++)
					archive & m_pDecimal->data[i];
							
				m_target.CopyFrom(*this);
			}
		}
	private:
		Decimal& m_target;
	};

	DecimalSerializer(decimal).serialize(archive);*/
		
	if (archive.isWrite())
	{
		archive & decimal.ToString(100);
	}
	else
	{
		std::wstring sDecimal;
		archive & sDecimal;
		decimal = Decimal::FromString(sDecimal);
	}
}

void serialize(SF::Archive &archive,CowSex& sex)
{
	if(archive.isWrite())
	{
		archive & sex.getCOWSEX();
	}
	else
	{
		COWSEX cowsex;
		archive & cowsex;
		sex = cowsex;
	}
}

void serialize(SF::Archive &archive,CowNo& cowNo)
{
	if(archive.isRead())
	{
		bool null;
		unsigned int countryIsoNum;
		std::wstring number;
		archive & null & countryIsoNum & number;
		if(!null)
			cowNo = CowNoFactory::Create(Countries::GetCountryByNumber(countryIsoNum),number);
		else
			cowNo.MakeNull();
	}
	else
	{
		archive & cowNo.IsNull() & cowNo.GetCountry().GetCountryISONumber() & cowNo.GetNumber();
	}
}


void serialize(SF::Archive &archive,Time& time)
{
	if(archive.isWrite())
	{		
		archive & time.IsNull();
		archive & time.GetSecsFromMidnight();
	}
	else
	{
		bool null;
		int secsFromMidnight;

		archive & null;
		archive & secsFromMidnight;

		if(null)
			time.MakeNull();
		else
			time.SetSecsFromMidnight(secsFromMidnight);		
	}
}

void serialize(SF::Archive &archive,NullInt& nullint)
{
	if(archive.isWrite())
	{		
		archive & nullint.IsNull();
		archive & nullint.ToInt();
	}
	else
	{
		bool null;
		int value;

		archive & null;
		archive & value;

		if(null)
			nullint.MakeNull();
		else
			nullint.operator=(value);
	}
}
