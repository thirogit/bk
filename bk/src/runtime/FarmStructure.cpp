#include "stdafx.h"
#include "FarmStructure.h"
#include <arrays/DeepDelete.h>

FarmStructure::FarmStructure(IFarm* farm)
{
	m_farm = new Farm(farm);
}

FarmStructure::~FarmStructure()
{
	delete m_farm;
	DeepDelete(m_herds);
}

void FarmStructure::AddHerd(IHerd* herd)
{

}

PtrEnumerator<IHerd> FarmStructure::EnumHerds() const
{
	return PtrEnumerator<IHerd>(m_herds);
}

IFarm* FarmStructure::GetFarm() const
{
	return m_farm;
}
