#include "stdafx.h"
#include "DocumentTerritoryObj.h"

DocumentTerritoryObj::DocumentTerritoryObj(IFarm* farm,IHerd* herd) : m_Farm(farm),m_Herd(herd)
{
}

DocumentTerritoryObj::~DocumentTerritoryObj()
{
	 
}

const IFarm* DocumentTerritoryObj::GetFarm() const
{
	return m_Farm;
}

const IHerd* DocumentTerritoryObj::GetHerd() const
{
	return m_Herd;
}

