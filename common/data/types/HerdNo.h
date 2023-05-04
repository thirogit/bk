#ifndef __HERDNO_H__
#define __HERDNO_H__

#include <string>
#include "../../types/Nullable.h"
#include <boost/optional.hpp>
#include <stdint.h>
#include "../../exception/TagException.h"

struct tag_HerdNoException{};
typedef TagException<tag_HerdNoException> HerdNoException;


class HerdNo;

class HerdNoFactory
{
public:
	static HerdNo Create(uint32_t herdIndex);	
};

class HerdNo : public Nullable
{
	HerdNo(uint32_t herdIndex);
public:
	HerdNo();
	HerdNo(const HerdNo& src);
	std::wstring ToString() const;
	HerdNo& operator=(const HerdNo& right);
	uint32_t GetIndex() const;
	void CopyFrom(const HerdNo& src);

	virtual void MakeNull();
	virtual bool IsNull() const;

private:
	boost::optional<uint32_t> m_HerdIndex;
	
	friend bool operator>(const HerdNo& left, const HerdNo& right);
	friend bool operator<(const HerdNo& left, const HerdNo& right);
	friend bool operator==(const HerdNo& left, const HerdNo& right);

	friend class HerdNoFactory;
};

#endif
