#if !defined(_IPASSPORTRECOGNITIONPROFILE_H_)
#define _IPASSPORTRECOGNITIONPROFILE_H_

#include "IPassportField.h"
#include <arrays/Enumerator.h>

class IPassportRecognitionProfile
{
public:
	virtual uint32_t GetId() const = 0;
    virtual int GetXDimention() const = 0;
    virtual int GetYDimention() const = 0;
    virtual const std::wstring& GetProfileName() const = 0;
	virtual IPassportField* GetFieldByName(const std::wstring& sFieldName) const = 0;
	virtual PtrEnumerator<IPassportField> EnumFields() const = 0;


};

#endif 

