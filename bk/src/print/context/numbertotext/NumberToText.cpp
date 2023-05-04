#include "stdafx.h"
#include "NumberToText.h"
#include <vector>
#include <boost/algorithm/string/join.hpp>

static wchar_t* numerals1[] =
{
	 L"zero",
     L"jeden",
     L"dwa",
     L"trzy",
     L"cztery",
     L"pięć",
     L"sześć",
     L"siedem",
     L"osiem",
     L"dziewięć",
     L"dziesięć",
     L"jedenaście",
     L"dwanaście",
     L"trzynaście",
     L"czternaście",
     L"piętnaście",
     L"szesnaście",
     L"siedemnaście",
     L"osiemnaście",
     L"dziewiętnaście"    
};

static wchar_t* numerals10[] =
{
	 L"",
	 L"",
	 L"dwadzieścia",
	 L"trzydzieści",
	 L"czterdzieści",
	 L"pięćdziesiąt",
	 L"sześćdziesiąt",
	 L"siedemdziesiąt",
	 L"osiemdziesiąt",
	 L"dziewięćdziesiąt"
};

static wchar_t* numerals100[] =
{
   L"",
   L"sto",
   L"dwieście",
   L"trzysta",
   L"czterysta",
   L"pięćset",
   L"sześćset",
   L"siedemset",
   L"osiemset",
   L"dziewięćset"
};

static wchar_t* numeral1000Declination[] =
{
	L"tysiąc",
	L"tysiące",
	L"tysięcy",
};

static wchar_t* numeral10_6Declination[] =
{
	 L"milion",
     L"miliony",
     L"milionów"
};

static wchar_t* numeral10_9Declination[] =
{
	L"miliard",
    L"miliardy",
    L"miliardów"
};



int NumberToText::DeclinationIndex(uint32_t number)
{
	if (number == 1) return 0;
	else
	{
		long tensReminder = number % 10;
		if (((number % 100) / 10) != 1 && tensReminder >= 2 && tensReminder <= 4) return 1;
	}
	return 2;
}

std::wstring NumberToText::Translate999(uint32_t number)
{
	std::vector<std::wstring> resultStack;

	if (number >= 1000) return L"";
	if (number == 0) return numerals1[0];

	uint32_t hundreds, hundredsReminder, tens, tensReminder;

	hundreds = number / 100;
	hundredsReminder = number % 100;

	if (hundreds > 0)
		resultStack.push_back(numerals100[(int)hundreds]);

	if (hundredsReminder > 0)
	{
		tens = hundredsReminder / 10;
		tensReminder = hundredsReminder % 10;

		if (tens > 1)
		{
			resultStack.push_back(numerals10[tens]);
			if (tensReminder > 0) resultStack.push_back(numerals1[tensReminder]);
		}
		else
		{
			resultStack.push_back(numerals1[(int)hundredsReminder]);
		}
	}

	return boost::algorithm::join(resultStack," ");
}

std::wstring NumberToText::Translate(uint32_t number)
{
	const uint32_t BILLION = 1000000000;
	const uint32_t MILLION = 1000000;
	const uint32_t THOUSAND = 1000;

	std::vector<std::wstring> resultStack;

	long billions, millions, thousands, thousandsReminder, billionsReminder, millionsReminder;
	
	billions = number / BILLION;
	billionsReminder = (number % BILLION);
	millions = billionsReminder / MILLION;
	millionsReminder = billionsReminder%MILLION;
	thousands = millionsReminder / THOUSAND;
	thousandsReminder = millionsReminder%THOUSAND;

	if (billions > 0)
	{
		resultStack.push_back(Translate999(billions));
		resultStack.push_back(numeral10_9Declination[DeclinationIndex(billions)]);
	}
	if (millions > 0)
	{
		resultStack.push_back(Translate999(millions));
		resultStack.push_back(numeral10_6Declination[DeclinationIndex(millions)]);
	}

	if (thousands > 0)
	{
		resultStack.push_back(Translate999(thousands));
		resultStack.push_back(numeral1000Declination[DeclinationIndex(thousands)]);
	}

	resultStack.push_back(Translate999(thousandsReminder));

	return boost::algorithm::join(resultStack, L" ");
}
