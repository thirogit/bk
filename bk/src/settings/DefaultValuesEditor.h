#pragma once

#include "DefaultValues.h"
#include "../context/XContext.h"

class DefaultValuesEditor
{
public:
	DefaultValuesEditor(XContext* context);
	void Load(DefaultValues& values);
	void Save(const DefaultValues& values);
private:
	XContext* m_context;
};




	

