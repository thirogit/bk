#ifndef __MUSTACHE_H__
#define __MUSTACHE_H__

#include <map>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include "../exception/TagException.h"

struct tag_MustacheException{};
typedef TagException<tag_MustacheException> MustacheException;

class MustacheStringStream
{
public:
	MustacheStringStream(const std::wstring& str);	
	std::wstring ReadUntil(wchar_t* stopCharacters);
	wchar_t GetChar();
	wchar_t LookAhead();
	bool IsEnd() const;
private:
	bool IsOneOf(wchar_t c,wchar_t *chs);
private:
	int m_Position;
	std::wstring m_String;
};

class MustacheVariable
{
public:
	MustacheVariable(const MustacheVariable& src);
	MustacheVariable(const std::wstring& sVariableName,const std::wstring& sValue);
	const std::wstring& GetValue() const;
	const std::wstring& GetName() const;
private:
	std::wstring m_sVariableValue;
	std::wstring m_sVariableName;
};

class MustacheVariables
{	
public:
	MustacheVariables();
	MustacheVariables(const MustacheVariables& src);
	~MustacheVariables();
	void Clear();

	MustacheVariables& Put(const std::wstring& variableName,const std::wstring& variableValue);
	MustacheVariables& Put(const MustacheVariable& variable);
	const MustacheVariable* Get(const std::wstring& sVariableName) const;
private:
	std::map<std::wstring, MustacheVariable*> m_VariablesMap;
};


class MustacheValueResolver
{
public:
	virtual std::wstring Get(const std::wstring& sVariableName) const = 0;
};

class MustacheSlice
{
public:
	virtual std::wstring GetSliceValue(MustacheValueResolver* resolver) = 0;
	virtual ~MustacheSlice() {};
};



class MustacheVariableSlice : public MustacheSlice
{
public:
	MustacheVariableSlice(const std::wstring& sSliceVariableName,int padLength,wchar_t padChar);
	virtual std::wstring GetSliceValue(MustacheValueResolver* resolver);
	virtual ~MustacheVariableSlice();
private:
	std::wstring m_SliceVariableName;
	int m_padLength;
	wchar_t m_padChar;
};

class MustacheConstantSlice : public MustacheSlice
{
public:
	MustacheConstantSlice(const std::wstring& sSliceValue);
	virtual std::wstring GetSliceValue(MustacheValueResolver* resolver);
	virtual ~MustacheConstantSlice();
private:
	std::wstring m_SliceValue;
};


class MustacheSlices
{
public:
	MustacheSlices();
	~MustacheSlices();
	void AddSlice(MustacheSlice* pSlice);
	int GetSliceCount() const;
	MustacheSlice* GetSlice(int iIndex);

private:
	std::vector<MustacheSlice*> m_Slices;
};

class MustacheVariablesValueResolver : public MustacheValueResolver
{
public:
	MustacheVariablesValueResolver(const MustacheVariables& variables);
	virtual std::wstring Get(const std::wstring& sVariableName) const;
private:
	const MustacheVariables& m_variables;
};


class Mustache;

class Mustache
{
public:
	static Mustache Parse(const std::wstring& str);
	std::wstring Format(const MustacheVariables& variables);
	std::wstring Format(MustacheValueResolver* variables);
	Mustache(const Mustache& src);
	~Mustache();
private:
	Mustache();
	static MustacheVariableSlice* CreateVariable(const std::wstring& variableToken);
private:
	boost::shared_ptr<MustacheSlices> m_Slices;
};

#endif
