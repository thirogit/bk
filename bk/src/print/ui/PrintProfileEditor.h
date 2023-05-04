#ifndef __PRINTPROFILEEDITOR_H__
#define __PRINTPROFILEEDITOR_H__

#include <afxwin.h>
#include "../../resource.h"
#include "../../context/XContext.h"
#include <configuration\print\PrintProfile.h>
#include <boost\optional.hpp>
#include <exception\TagException.h>


struct tag_PrintProfileEditorExeption {};
typedef TagException<tag_PrintProfileEditorExeption> PrintProfileEditorExeption;

typedef boost::optional<PrintOption> DubiousPrintOption;

class PrintProfileEditor 
{
public:
	PrintProfileEditor(XContext* context);
	~PrintProfileEditor();
	void New(const std::wstring& documentCode);
	void Edit(uint32_t profileId);
	void Save();
	bool IsUpdating() const;	

	const std::wstring& GetProfileName() const;
	void SetProfileName(const std::wstring& profileName);
	void SetOption(const PrintOption& option);
	void RemoveOption(const std::wstring& optionName);
	DubiousPrintOption GetOption(const std::wstring& optionName);

	OrderSpecifier* GetOrderSpecifierAt(int index);
	void DeleteOrderSpecifierAt(int index);
	void AddOrderSpecifier(const OrderSpecifier& specifier);
	int GetOrderSpecifiersCount() const;
	void ClearOrderSpecifiers();


protected:
	void throwIfNotInitialized() const;
private:
	PrintProfile* m_workingCopy;
	XContext* m_context;
};

#endif