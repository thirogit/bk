#ifndef __RECOGNITIONITEMVIEW_H__
#define __RECOGNITIONITEMVIEW_H__

#include <data\runtime\ItemId.h>
#include <types\Decimal.h>
#include <data\types\CowNo.h>
#include <data\types\CowSex.h>
#include <types\DateTime.h>


namespace RecognitionItemViewMembers
{
	enum RecognitionItemViewMember
	{
		RecognitionItemView_CowNo,
		RecognitionItemView_PassNo,
		RecognitionItemView_FirstOwner,
		RecognitionItemView_BirthDate,
		RecognitionItemView_Weight,
		RecognitionItemView_Sex,
		RecognitionItemView_Stock,
		RecognitionItemView_Class		
	};
};


class RecognitionItemView
{
public:
	RecognitionItemView(ItemId id);

	ItemId GetId() const;

	const Decimal& GetWeight() const;
	void SetWeight(const Decimal& weight);

	const std::wstring& GetStockCd() const;
	void SetStockCd(const std::wstring& stockCd);

	const std::wstring& GetClassCd() const;
	void SetClassCd(const std::wstring& classCd);

	const CowNo& GetCowNo() const;
	void SetCowNo(const CowNo& cowNo);

	const CowNo& GetMotherNo() const;
	void SetMotherNo(const CowNo& motherNo);

	const DateTime& GetBirthDate() const;
	void SetBirthDate(const DateTime& birthDt);

	const std::wstring& GetPassNo() const;
	void SetPassNo(const std::wstring& passNo);

	const std::wstring& 	GetHealthCertNo() const;
	void SetHealthCertNo(const std::wstring& sHealthCertNo);

	const DateTime&	GetPassDate() const;
	void SetPassDate(const DateTime& passDt);

	const std::wstring& GetBirthPlace() const;
	void SetBirthPlace(const std::wstring& birthPlace);

	const std::wstring& 	GetExtras() const;
	void SetExtras(const std::wstring& sExtras);

	const std::wstring& GetFirstOwnerAlias() const;
	void SetFirstOwnerAlias(const std::wstring& alias);

	const CowSex& GetSex() const;
	void SetSex(const CowSex& cowsex);
private:
	ItemId 			m_Id;
	Decimal			m_weight;
	std::wstring	m_stockCd;
	std::wstring	m_classCd;
	CowNo			m_cowno;
	DateTime		m_birthdate;
	std::wstring	m_birthplace;
	CowSex			m_sex;
	CowNo			m_motherNo;
	std::wstring	m_passno;
	DateTime		m_passdate;
	std::wstring	m_fstownerAlias;
	std::wstring	m_extras;
};

#endif