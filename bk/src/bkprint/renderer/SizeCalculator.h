#ifndef __SIZECALCULATOR_H__
#define __SIZECALCULATOR_H__

#include "../template/FieldsContainer.h"

namespace bkprint
{
	class SizeCalculator
	{
	public:
		SizeCalculator(const FieldsContainer& fields);		
		int CalculateSouthParallel();
		int CalculateNorthParallel();
		int CalculateWestMeridian();
		int CalculateEastMeridian();
		int GetHeight();
		int GetWidth();
		int GetNorth();
		int GetSouth();
		int GetWest();
		int GetEast();
	private:
		const FieldsContainer& m_fields;
		int m_north;
		int m_south;
		int m_west;
		int m_east;
	};
}


#endif