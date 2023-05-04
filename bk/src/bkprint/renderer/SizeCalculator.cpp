#include "stdafx.h"
#include "SizeCalculator.h"

namespace bkprint
{
	SizeCalculator::SizeCalculator(const FieldsContainer& fields) : m_fields(fields)
	{
		m_north = CalculateNorthParallel();
		m_south = CalculateSouthParallel();
		m_west = CalculateWestMeridian();
		m_east = CalculateEastMeridian();
	}
	
	int SizeCalculator::CalculateSouthParallel()
	{
		int maxSouth = 0;
		PtrEnumerator<Field> enumFields = m_fields.EnumFields();
		Field* field;
		while (enumFields.hasNext())
		{
			field = *enumFields;
			int fieldBottom = field->GetPosition().y + field->GetSize().cy;
			if(fieldBottom > maxSouth)
			{
				maxSouth = fieldBottom;
			}
			enumFields.advance();
		}
		return maxSouth;
	}

	int SizeCalculator::CalculateNorthParallel()
	{
		return 0;
	}

	int SizeCalculator::CalculateWestMeridian()
	{
		return 0;
	}

	int SizeCalculator::CalculateEastMeridian()
	{
		int maxEast = 0;
		PtrEnumerator<Field> enumFields = m_fields.EnumFields();
		Field* field;
		while (enumFields.hasNext())
		{
			field = *enumFields;
			int fieldRight = field->GetPosition().x+field->GetSize().cx;
			if(fieldRight > maxEast)
			{
				maxEast = fieldRight;
			}
			enumFields.advance();
		}
		return maxEast;
	}


	int SizeCalculator::GetHeight()
	{
		return GetSouth() - GetNorth();
	}

	int SizeCalculator::GetWidth()
	{
		return GetEast() - GetWest();
	}

	int SizeCalculator::GetNorth()
	{
		return m_north;
	}

	int SizeCalculator::GetSouth()
	{
		return m_south;
	}

	int SizeCalculator::GetWest()
	{
		return m_west;
	}

	int SizeCalculator::GetEast()
	{
		return m_east;
	}
}
