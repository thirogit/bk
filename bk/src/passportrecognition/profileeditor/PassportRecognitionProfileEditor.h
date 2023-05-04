#if !defined(_PASSPORTRECOGNITIONPROFILEEDITOR_H_)
#define _PASSPORTRECOGNITIONPROFILEEDITOR_H_

#include "PassportFieldEditor.h"
#include <map>
#include "../../context/XContext.h"
#include <exception/TagException.h>

struct tag_PassportRecognitionProfileEditorException{};

typedef TagException<tag_PassportRecognitionProfileEditorException> PassportRecognitionProfileEditorException;

class PassportRecognitionProfileEditor
{
	class PassportFieldEditorImpl : public PassportFieldEditor
	{
	public:
		PassportFieldEditorImpl(PassportRecognitionProfile* profile, const std::wstring& fieldName);

		virtual IPassportField* GetField() const;
		virtual void SetW(int w);
		virtual void SetH(int h);
		virtual void SetX(int x);
		virtual void SetY(int y);

	private:
		PassportRecognitionProfile* m_profile;
		std::wstring m_fieldName;
	};

public:
	PassportRecognitionProfileEditor(XContext* context);
	~PassportRecognitionProfileEditor();
public:
	void New();
	void Edit(uint32_t profileId);
	bool IsEditing() const;

	IPassportRecognitionProfile* GetProfile() const;

    void SetXDimention(int xdim);
    void SetYDimention(int ydim);	
    void SetProfileName(const std::wstring& sProfileName);
	PassportFieldEditor* EditField(const std::wstring& sFieldName);
	
	void Save();
protected:
	void ThrowIfNotInitialized() const;
	void ThrowIfAlreadyInitialized() const;
	void CreateFieldEditors();

protected:
	XContext* m_context;
    std::map<std::wstring,PassportFieldEditor*> m_EditorsMap;
	PassportRecognitionProfile* m_pWorkingCopy;
	
};

#endif 

