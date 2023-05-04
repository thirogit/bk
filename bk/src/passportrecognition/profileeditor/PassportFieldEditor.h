#ifndef __PASSPORTFIELDEDITOR_H__
#define __PASSPORTFIELDEDITOR_H__

#include <configuration/passportrecognition/IPassportField.h>

class PassportFieldEditor
{
public:
	virtual IPassportField* GetField() const = 0;	
	virtual void SetW(int w) = 0;
	virtual void SetH(int h) = 0;
	virtual void SetX(int x) = 0;
	virtual void SetY(int y) = 0;
};


#endif 