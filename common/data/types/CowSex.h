#ifndef __COWSEX_H__
#define __COWSEX_H__

#include <string>

enum COWSEX
{
	SEX_XX = 1, //female
	SEX_UNK = 0, //unknown
	SEX_XY = -1 //male
};

class CowSex
{
public:
	CowSex(COWSEX sex = SEX_UNK);
	void MakeXX();
	bool IsXX() const;
	void MakeXY();
	bool IsXY() const;
	void MakeUNK();
	bool IsUNK() const;
	void Make(COWSEX eSex);
	CowSex& operator=(const CowSex &right);
	CowSex& operator=(COWSEX rSex);	
	COWSEX getCOWSEX() const;
	bool IsCowSex(COWSEX eSex) const;
	std::wstring ToString() const;
	friend bool operator>=(const CowSex& left,const CowSex& right);
	friend bool operator<=(const CowSex& left,const CowSex& right);
	friend bool operator>(const CowSex& left,const CowSex& right);
	friend bool operator<(const CowSex& left,const CowSex& right);
	friend bool operator==(const CowSex& left,const CowSex& right);
private:
	COWSEX m_sex;
};

#endif
