#ifndef __FARMSTRUCTURE_H__
#define __FARMSTRUCTURE_H__

#include <data/impl/Farm.h>
#include <data/impl/Herd.h>
#include <arrays/Enumerator.h>
#include <vector>

class FarmStructure
{
public:
	FarmStructure(IFarm* farm);
	~FarmStructure();
	void AddHerd(IHerd* herd);
	IHerd* GetHerd(uint32_t herdId) const;
	PtrEnumerator<IHerd> EnumHerds() const;
	IFarm* GetFarm() const;
private:
	Farm* m_farm;
	std::vector<Herd*> m_herds;
};


#endif