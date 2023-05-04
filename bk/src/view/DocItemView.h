#ifndef __DOCITEMVIEW_H__
#define __DOCITEMVIEW_H__

#include <data\runtime\ItemId.h>
#include <data\types\CowNo.h>
#include <data\types\CowSex.h>
#include <types\DateTime.h>
#include <types\Decimal.h>
#include <types\NullInt.h>
#include "ColoredObject.h"

struct DocItemViewMember
{
	enum
	{
		DocItemView_CowNo,
		DocItemView_PassNo,
		DocItemView_FirstOwner,
		DocItemView_BirthDate,
		DocItemView_Weight,
		DocItemView_Sex,
		DocItemView_Stock,
		DocItemView_Class,
		DocItemView_Group
	};
};


class DocItemView 
{
public:
	DocItemView(ItemId id);
	
	const ItemId& GetItemId() const;
	const CowNo& GetCowNo() const;	
	const std::wstring& GetPassNo() const;
	const std::wstring& GetFstOwnerAlias() const;
	const DateTime& GetBirthDate() const;
	const ColoredObject<Decimal>& GetWeight() const;
	const CowSex& GetSex() const;
	const ColoredObject<std::wstring>& GetStock() const;
	const ColoredObject<std::wstring>& GetClass()const;
	const ColoredObject<NullInt>& GetGroup() const;
	
	void SetCowNo(const CowNo& cowNo);	
	void SetPassNo(const std::wstring& passNo);
	void SetFstOwnerAlias(const std::wstring& fstOwnerAlias);
	void SetBirthDate(const DateTime& birthDt);
	void SetWeight(const ColoredObject<Decimal>& weight);
	void SetSex(const CowSex& sex);
	void SetStock(const ColoredObject<std::wstring>& stockCd);
	void SetClass(const ColoredObject<std::wstring>& classCd);
	void SetGroup(const ColoredObject<NullInt>& group);
	
private:
	ItemId m_id;
	CowNo m_cowNo;
	std::wstring m_passNo;
	std::wstring m_fstOwnerAlias;
	DateTime m_birthDt;
	ColoredObject<Decimal> m_weight;
	CowSex m_sex;
	ColoredObject<std::wstring> m_stock;
	ColoredObject<std::wstring> m_class;
	ColoredObject<NullInt> m_group;
};

#endif