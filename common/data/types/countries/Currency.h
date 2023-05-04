#ifndef __CURRENCY_H__
#define __CURRENCY_H__

class Currency : public Nullable
{
public:
	Country(const Country& copy);
	Country& operator=(const Country& right);

	unsigned int GetCountryISONumber() const;
	bool IsCountryInEU() const;
	std::string GetCountryCode2A() const;
	std::string GetCountryCode3A() const;
	std::string GetCurrencyCode() const;
	int GetCowNoLength() const;
	int GetFarmNoLength() const;
	int GetHerdNoLength() const;
		
protected:
	Country(unsigned int number);
	void	SetCountryInEU(bool bInEU);
	void	SetCountryCode2A(const std::string& sCountryCode2a);
	void	SetCountryCode3A(const std::string& sCountryCode3a);
	void	SetCurrencyCode(const std::string& sCurrencyCode);
	void    SetCowNoLength(int length);
	void    SetFarmNoLength(int length);
	void    SetHerdNoLength(int length);
	
private:
	
	std::string code2a;
	std::string code3a;
	unsigned int isoNumber;
	bool		inEU;
	std::string currencyCode;
	int			cowNoLength;
	int			farmNoLength;
	int			herdNoLength;

	friend class Countries;	
};

class InvalidCountry : public Country
{
public:
	InvalidCountry();
};


#endif